#include "stdafx.h"
#include <chrono>
#include "FrontPanelRC1101.h"
#include "FrontPanelNative.h"

namespace {
    const int NUMBER_OF_ENCODERS = 7;
    const auto MESSAGE_MIN_MSEC = std::chrono::milliseconds(50); // throttle update messages to this often
    const auto FOLLOWUP_FREQUENCY_TIMER_MSEC = std::chrono::milliseconds(950);
}

FrontPanelRC1101::FrontPanelRC1101()
    : m_running(false)
    , m_stop(false)
    , m_encCenters(NUMBER_OF_ENCODERS)
    , m_brightness(0)
    , m_continueUpdating(false)
{}

HRESULT FrontPanelRC1101::FinalConstruct()
{
    return CoCreateFreeThreadedMarshaler(
        GetControllingUnknown(), &m_pUnkMarshaler.p);
}

HRESULT FrontPanelRC1101::Initialize(IUnknown *pRig)
{
    if (m_running)
        return E_UNEXPECTED;
    IStream *pStream(0);
    HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(__uuidof(m_pRig), pRig, &pStream);
    if (SUCCEEDED(hr))
    {
        m_thread = std::thread(std::bind(&FrontPanelRC1101::threadEntry, this, pStream));
        lock_t l(m_mutex);
        while (!m_running)
            m_cond.wait(l);
        if (m_pRig)
            return S_OK;
        hr = E_FAIL;
    }
    return hr;
}

void FrontPanelRC1101::Disconnect()
{
    if (m_running)
    {
        addToQueue([](FrontPanelRC1101*p)
            {
                if (p->m_pRig)
                    p->m_pRig->EndRemoteControl();
                p->m_pRig.Release();
            }
        );
    }
}

void FrontPanelRC1101::FinalRelease()
{
    {
        lock_t l(m_mutex);
        m_stop = true;
        m_cond.notify_all();
    }
    if (m_thread.joinable())
        m_thread.join();
    m_pUnkMarshaler.Release();
}

void FrontPanelRC1101::threadEntry(IStream *pRigStream)
{
    ::CoInitializeEx(0, COINIT_MULTITHREADED);
    ::CoGetInterfaceAndReleaseStream(pRigStream, __uuidof(m_pRig), (void**)&m_pRig);
    pRigStream = 0;
    {
        lock_t l(m_mutex);
        m_running = true;
        m_cond.notify_all();
    }
    if (m_pRig)
    {
        std::chrono::steady_clock::time_point lastSent;
        UpdateFcn_t throttledUpdate;
        UpdateFcn_t updateAgain;
        m_pRig->RequestInitializeControls();
        if (m_brightness != 0)
            m_frontPanel->SetTrellisBrightness(m_brightness);

        try {
            std::vector<short> enow(NUMBER_OF_ENCODERS);
            std::vector<short> eprev(NUMBER_OF_ENCODERS);
            unsigned short switches = 0; unsigned short switchesprev = 0;
            byte encswitch = 0;  byte encswitchesprev = 0;

            for (; !m_stop;)
            {
                {   // do one command each time around the loop
                    lock_t l(m_mutex);
                    if (!m_queue.empty())
                    {
                        threadEntry_t f = m_queue.front();
                        m_queue.pop_front();
                        l.unlock();
                        f(this);
                    }
                    else if (!m_continueUpdating)
                        m_cond.wait(l);
                }

                unsigned short numenc = 0;
                bool ok = m_frontPanel->GetInputState(&enow[0], switches, encswitch);
                std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                if (ok)
                {
                    if (m_continueUpdating)
                    {
                        int i = NUMBER_OF_ENCODERS - 1;
                        for (; i >= 0; i--)
                        {   // scan backwards
                            if (eprev[i] != enow[i])
                                break;
                        }
                        if ((i >= 0) ||
                            (switches != switchesprev) ||
                            (encswitch != encswitchesprev))
                        {
                            std::vector<LONG> update(i + 1);
                            for (int j = 0; j <= i; j++)
                                update[j] = (LONG)(m_encCenters[j] + enow[j]);
                            unsigned short sw = switches;
                            byte encs = encswitch;
                            ATL::CComPtr<IWlRemoteRig> pRig = m_pRig;
                            UpdateFcn_t updateDelegate = [pRig, update, sw, encs]()
                            {
                                if (pRig)
                                {
                                    LONG *pUpdate = 0;
                                    if (!update.empty()) // std::vector checks
                                        pUpdate = const_cast<LONG*>(&update[0]);
                                    pRig->ControlsChanged((USHORT)update.size(),
                                        pUpdate, sw, encs);
                                }
                            };

                            // either invoke it or defer it
                            bool defer = (now - lastSent) < MESSAGE_MIN_MSEC;
                            if (defer)
                                throttledUpdate = updateDelegate;
                            else
                            {
                                updateDelegate();
                                throttledUpdate = UpdateFcn_t();
                                lastSent = now;
                            }
                            updateAgain = updateDelegate; // this delegate gets used twice
                        }
                    }

                    eprev.assign(enow.begin(), enow.end());
                    switchesprev = switches;
                    encswitchesprev = encswitch;
                }
                if (throttledUpdate && ((now - lastSent) >= MESSAGE_MIN_MSEC))
                {
                    throttledUpdate();
                    lastSent = now;
                    throttledUpdate = UpdateFcn_t();
                }
                else if (updateAgain &&
                    (now - lastSent) > FOLLOWUP_FREQUENCY_TIMER_MSEC)
                {
                    updateAgain();
                    updateAgain = UpdateFcn_t();
                    lastSent = now;
                }
            }
        }
        catch (...)
        {
        }
    }

    if (m_pRig)
        m_pRig->EndRemoteControl();
    m_pRig.Release();
    ::CoUninitialize();
    lock_t l(m_mutex);
    m_queue.clear();
    m_running = false;
    m_cond.notify_all();
}

// IWlControlSite
HRESULT FrontPanelRC1101::ContinueUpdating()
{
    {
        lock_t l(m_mutex);
        m_continueUpdating = true;
        m_cond.notify_all();
    }
    return S_OK;
}

void FrontPanelRC1101::addToQueue(const threadEntry_t &te)
{
    lock_t l(m_mutex);
    m_queue.push_back(te);
    m_cond.notify_all();
}

HRESULT FrontPanelRC1101::SetEncoderCenter(USHORT which, LONG center)
{
    addToQueue([which, center](FrontPanelRC1101 *fp) {
        fp->m_frontPanel->SetEncoderCenter(which, (unsigned)center);
    });
    return S_OK;
}

HRESULT FrontPanelRC1101::PressTrellisButton(USHORT which, USHORT value)
{
    addToQueue([which, value](FrontPanelRC1101 *fp) {
        fp->m_frontPanel->PressTrellisButton((byte)which, (byte)value);
    });
    return S_OK;
}

HRESULT FrontPanelRC1101::SetDisplayString(USHORT which, BSTR  value)
{
    BOOL defChar(FALSE);
    int cvtCount = ::WideCharToMultiByte(CP_ACP, 0, value, -1, 0, 0, 0, &defChar);
    std::vector<char> snmb(cvtCount);
    ::WideCharToMultiByte(CP_ACP, 0, value, -1, &snmb[0], snmb.size(), 0, &defChar);
    addToQueue([which, snmb](FrontPanelRC1101 *fp) {
        fp->m_frontPanel->SetStringObject((byte)which, &snmb[0]);
    });
    return S_OK;
}

HRESULT FrontPanelRC1101::SetupTrellisGroup(USHORT which, USHORT mask, USHORT objType, USHORT objIndex)
{
    addToQueue([which, mask, objType, objIndex](FrontPanelRC1101 *fp) {
        fp->m_frontPanel->SetTrellisGroup((byte)which, mask, objType, objIndex);
    });
    return S_OK;
}

HRESULT FrontPanelRC1101::SetEncoderMap(USHORT which, USHORT objTypeL, USHORT objIndexL,
    USHORT objTypeH, USHORT objIndexH, SHORT mult, LONG lowLimit, LONG hiLimit)
{
    addToQueue([which, objTypeL, objIndexL, objTypeH, objIndexH, mult, lowLimit, hiLimit]
    (FrontPanelRC1101 *fp) {
        fp->m_frontPanel->SetEncoderMap((byte)which, objTypeL, objIndexL, objTypeH, objIndexH,
            mult, lowLimit, hiLimit);
    });
    return S_OK;
}

HRESULT FrontPanelRC1101::SetDisplayObjects(USHORT count, USHORT objTypes[],
    USHORT objIndices[],
    USHORT values[])
{
    std::vector<RadioPanelUsb::CFrontPanel::SetDisplayObject>
        objects(RadioPanelUsb::CFrontPanel::NUM_DISPLAY_OBJECTS_IN_I2C_MESSAGE);
    for (unsigned i = 0; i < count; i++)
    {
        objects[i].objType = objTypes[i];
        objects[i].objIdx = objIndices[i];
        objects[i].value = values[i];
    }
    addToQueue([objects]
    (FrontPanelRC1101 *fp) {
        fp->m_frontPanel->SetDisplayObjects(&objects[0]);
    });
    return S_OK;
}

HRESULT FrontPanelRC1101::ResetDisplayDefaults()
{
    addToQueue([](FrontPanelRC1101 *fp) {
        fp->m_frontPanel->ResetDisplayDefaults();
    });
    return S_OK;
}

HRESULT FrontPanelRC1101::SetEncoderSwitchState(USHORT s)
{
    addToQueue([s](FrontPanelRC1101 *fp) {
        fp->m_frontPanel->SetEncoderSwitchState((byte)s);
    });
    return S_OK;
}

