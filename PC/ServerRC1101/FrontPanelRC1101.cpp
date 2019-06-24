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
    , m_haveResetDisplayDefaults(false)
{}

HRESULT FrontPanelRC1101::FinalConstruct()
{
    HRESULT hr = CoCreateFreeThreadedMarshaler(
        GetControllingUnknown(), &m_pUnkMarshaler.p);
    return hr;
}

HRESULT FrontPanelRC1101::Initialize(IUnknown *pRig)
{
    if (m_running)
        return E_UNEXPECTED;

    IStream *pStream(0);
    HRESULT
        hr = ::CoMarshalInterThreadInterfaceInStream(__uuidof(IWlRemoteRig), pRig, &pStream);
    if (SUCCEEDED(hr))
    {
        m_thread = std::thread(std::bind(&FrontPanelRC1101::threadEntry, this, pStream));
        lock_t l(m_mutex);
        while (!m_running && !m_stop)
            m_cond.wait(l);
        if (m_running)
            return S_OK;
        hr = E_FAIL;
    }
    return hr;
}

void FrontPanelRC1101::Disconnect()
{
    lock_t l(m_mutex);
    m_stop = true;
    m_cond.notify_all();
}

void FrontPanelRC1101::FinalRelease()
{
    /* complicated by the fact that this call might happen
    ** either on our own thread or from a foreign thread.
    ** std::thread has to be handled differently for those
    ** two cases because join() on self deadlocks,
    ** while delete on self while thread is running aborts.
    */
    {
        lock_t l(m_mutex);
        m_stop = true;
        m_cond.notify_all();
        while (m_running)
            m_cond.wait(l);
    }
    if (m_thread.joinable())
    {
        if (m_thread.get_id() == std::this_thread::get_id())
            m_thread.detach();
        else
            m_thread.join();
    }
    m_pUnkMarshaler.Release();
}

void FrontPanelRC1101::threadEntry(IStream *pStream)
{
    ::CoInitializeEx(0, COINIT_MULTITHREADED);
    {
        ATL::CComPtr<FrontPanelRC1101> refSelf(this);
        ATL::CComPtr<IWlRemoteRig> pRig;
        HRESULT hr = ::CoGetInterfaceAndReleaseStream(pStream, __uuidof(pRig), (void**)&pRig);
        {
            lock_t l(m_mutex);
            m_cond.notify_all();
            if (SUCCEEDED(hr))
                m_running = true;
            else
            {
                m_stop = true;
                return;
            }
        }
        std::chrono::steady_clock::time_point lastSent;
        UpdateFcn_t throttledUpdate;
        UpdateFcn_t updateAgain;
        pRig->RequestInitializeControls();
        if (m_brightness != 0)
            m_frontPanel->SetTrellisBrightness(m_brightness);

        try {
            std::vector<short> enow(NUMBER_OF_ENCODERS);
            std::vector<short> eprev(NUMBER_OF_ENCODERS);
            unsigned short switches = 0;
            unsigned short switchesprev = 0;
            byte encswitch = 0;
            byte encswitchesprev = 0;
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
                            auto encs = encswitch;
                            UpdateFcn_t updateDelegate = [pRig, update, sw, encs]()
                            {
                                if (pRig)
                                {
                                    LONG *pUpdate = 0;
                                    if (!update.empty()) // std::vector checks
                                        pUpdate = const_cast<LONG*>(&update[0]);
                                    pRig->ControlsChanged(
                                        (USHORT)update.size(), pUpdate, sw, encs);
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
        lock_t l(m_mutex);
        m_running = false;
        auto queue = m_queue; // copy under lock
        m_queue.clear();
        m_cond.notify_all();
        l.unlock(); // refSelf destructor must happen without lock
        queue.clear(); // remove any references while not locked
    }
    ::CoUninitialize();
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
    if (!m_stop)
    {
        m_queue.push_back(te);
        m_cond.notify_all();
    }
}

HRESULT FrontPanelRC1101::SetEncoderCenter(USHORT which, LONG center)
{
    if (!m_haveResetDisplayDefaults)
        return S_FALSE;
    if (which >= NUMBER_OF_ENCODERS)
        return E_INVALIDARG;
    {   // these come in faster than can be handled.
        // overwrite earlier ones with later ones.
        lock_t l(m_mutex);
        m_encoderCenterQueue[which] = 
            [which, center](FrontPanelRC1101 *fp) {
                fp->m_encCenters[which] = center;
                for (int i = 0; i < 3; i++)
                {
                    if (fp->m_frontPanel->SetEncoderCenter(which, (unsigned)center))
                        break;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
        };
    }
    addToQueue([which](FrontPanelRC1101 *p) 
        {
            threadEntry_t f;
            {
                lock_t l(p->m_mutex);
                auto itor = p->m_encoderCenterQueue.find(which);
                if (itor != p->m_encoderCenterQueue.end())
                {
                    f = itor->second;
                    p->m_encoderCenterQueue.erase(itor);
                }
            }
            if (f)
                f(p);
        });
    return S_OK;
}

HRESULT FrontPanelRC1101::PressTrellisButton(USHORT which, USHORT value)
{
    if (!m_haveResetDisplayDefaults)
        return S_FALSE;
    addToQueue([which, value](FrontPanelRC1101 *fp) {
        fp->m_frontPanel->PressTrellisButton((byte)which, (byte)value);
    });
    return S_OK;
}

HRESULT FrontPanelRC1101::SetDisplayString(USHORT which, BSTR  value)
{
    if (!m_haveResetDisplayDefaults)
        return S_FALSE;
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
    addToQueue(
        [which, objTypeL, objIndexL, objTypeH, objIndexH, mult, lowLimit, hiLimit]
        (FrontPanelRC1101 *fp) 
            {
            fp->m_frontPanel->SetEncoderMap(
                (byte)which, objTypeL, objIndexL, objTypeH, objIndexH, mult, lowLimit, hiLimit);
            });
    return S_OK;
}

HRESULT FrontPanelRC1101::SetDisplayObjects(USHORT count, USHORT objTypes[],
    USHORT objIndices[], USHORT values[])
{
    if (!m_haveResetDisplayDefaults)
        return S_FALSE;
    if (count > 0)
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
    }
    return S_OK;
}

HRESULT FrontPanelRC1101::ResetDisplayDefaults()
{
    m_haveResetDisplayDefaults = true;
    addToQueue([](FrontPanelRC1101 *fp) {
        fp->m_frontPanel->ResetDisplayDefaults();
        // block the processing queue for this long-to process command
        std::this_thread::sleep_for(std::chrono::seconds(3));
    });
    return S_OK;
}

HRESULT FrontPanelRC1101::SetEncoderSwitchState(USHORT s)
{
    if (!m_haveResetDisplayDefaults)
        return S_FALSE;
    addToQueue([s](FrontPanelRC1101 *fp) {
        fp->m_frontPanel->SetEncoderSwitchState((byte)s);
    });
    return S_OK;
}

