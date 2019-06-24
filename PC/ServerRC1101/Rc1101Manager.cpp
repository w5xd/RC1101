// Rc1101Manager.cpp : Implementation of CRc1101Manager

#include "stdafx.h"
#include "Rc1101Manager.h"
#include "FrontPanelNative.h"

// CRc1101Manager

CRc1101Manager::CRc1101Manager()
{}

HRESULT CRc1101Manager::FinalConstruct()
{
    return S_OK;
}

void CRc1101Manager::FinalRelease()
{
    auto all = m_frontPanels; // copy reference counts
    for (auto &fp : all)
    {
        if (fp.inUse)
            fp.inUse->Disconnect();
    }
}

HRESULT CRc1101Manager::ListDevices(ULONG *pVal)
{
    auto all = m_frontPanels; // copy reference counts
    for (auto &fp : all)
    {
        if (fp.inUse)
            fp.inUse->Disconnect();
    }
    m_frontPanels.clear();
    all.clear();
    DWORD nc;
    FT_STATUS stat = (*Ftd2XXDynamic::FT_CreateDeviceInfoList)(&nc);
    if ((FT_OK == stat) && nc > 0)
    {
        std::vector<FT_DEVICE_LIST_INFO_NODE> nodes(nc);
        stat = (*Ftd2XXDynamic::FT_GetDeviceInfoList)(&nodes[0], &nc);
        for (int idx = 0; idx < static_cast<int>(nc); idx++)
        {
            if (nodes[idx].Flags & FT_FLAGS_OPENED)
                continue;
            std::string sn(&nodes[idx].SerialNumber[0]);
            FT_HANDLE handle;
            stat = (*Ftd2XXDynamic::FT_OpenEx)((void *)sn.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &handle);
            if (stat == FT_OK)
            {
                std::shared_ptr<RadioPanelUsb::CFrontPanel> f(new RadioPanelUsb::CFrontPanel(handle));
                if (f->isFrontPanel() == RadioPanelUsb::CFrontPanel::DEV_OK)
                    m_frontPanels.push_back(FrontPanel(sn, f));
            }
        }
    }
    *pVal = m_frontPanels.size();
    return S_OK;
}

HRESULT CRc1101Manager::OpenByUsbSerialNumber(BSTR sn, ULONG *pIndex)
{
    BOOL defChar(FALSE);
    int cvtCount = ::WideCharToMultiByte(CP_ACP, 0, sn, -1, 0, 0, 0, &defChar);
    std::vector<char> snmb(cvtCount);
    ::WideCharToMultiByte(CP_ACP, 0, sn, -1, &snmb[0], snmb.size(), 0, &defChar);
    *pIndex = (ULONG)-1;
    FT_HANDLE handle;
    FT_STATUS stat = (*Ftd2XXDynamic::FT_OpenEx)((PVOID)&snmb[0],
        FT_OPEN_BY_SERIAL_NUMBER, &handle);
    if (stat == FT_OK)
    {
        std::shared_ptr<RadioPanelUsb::CFrontPanel> f(new RadioPanelUsb::CFrontPanel(handle));
        if (f->isFrontPanel() == RadioPanelUsb::CFrontPanel::DEV_OK)
        {
            *pIndex = m_frontPanels.size();
            m_frontPanels.push_back(FrontPanel(&snmb[0], f));
            return S_OK;
        }
        return E_UNEXPECTED;
    }
    return E_FAIL;
}

HRESULT CRc1101Manager::ResetAndOpenDevice(BSTR sn, ULONG *pIndex)
{
    BOOL defChar(FALSE);
    int cvtCount = ::WideCharToMultiByte(CP_ACP, 0, sn, -1, 0, 0, 0, &defChar);
    std::vector<char> snmb(cvtCount);
    ::WideCharToMultiByte(CP_ACP, 0, sn, -1, &snmb[0], snmb.size(), 0, &defChar);
    *pIndex = (ULONG)-1;
        static const DWORD RESET_WAIT_MSEC = 10000;
        static const unsigned MAX_AROUND = 2;
        for (unsigned i =0; i < MAX_AROUND; i++)
        {
            FT_HANDLE handle = 0;
            FT_STATUS stat = (*Ftd2XXDynamic::FT_OpenEx)((PVOID)&snmb[0],
                    FT_OPEN_BY_SERIAL_NUMBER, &handle);
            if (stat == FT_OK)
            {
                std::shared_ptr<RadioPanelUsb::CFrontPanel> f(new RadioPanelUsb::CFrontPanel(handle)); // will close it
                RadioPanelUsb::CFrontPanel::FailureType_t res = f->isFrontPanel();
                switch (res)
                {
                    case RadioPanelUsb::CFrontPanel::DEV_FAILED_SETUP:
                    case RadioPanelUsb::CFrontPanel::DEV_FAILED_FT232H_SYNC:
                    case RadioPanelUsb::CFrontPanel::DEV_FAILED_I2C_SETUP:
                        (*Ftd2XXDynamic::FT_CyclePort)(handle);
                        f.reset();   // FT_Close will be done in CFrontPanel destructor
                        ::Sleep(RESET_WAIT_MSEC);
                        break;

                    case RadioPanelUsb::CFrontPanel::DEV_FAILED_ID_YOURSELF:
                        f->ResetFrontPanel();
                        ::Sleep(RESET_WAIT_MSEC);
                        if (f->isFrontPanel() != RadioPanelUsb::CFrontPanel::DEV_OK)
                            break;
                        // else fall through
                    case RadioPanelUsb::CFrontPanel::DEV_OK:
                        *pIndex = m_frontPanels.size();
                        m_frontPanels.push_back(FrontPanel(&snmb[0], f));
                        return S_OK;
                }
            }
            else
                break;
        }
        return E_FAIL;
}

HRESULT CRc1101Manager::GetDevice( ULONG idx, IUnknown **ppOut)
{
    *ppOut = 0;
    if (idx >= m_frontPanels.size())
        return E_INVALIDARG;

    if (m_frontPanels[idx].inUse)
        return m_frontPanels[idx].inUse->QueryInterface(IID_IUnknown, (void**)ppOut);

    CComObject<FrontPanelRC1101> *pObj = 0;
    HRESULT hr = CComObject<FrontPanelRC1101>::CreateInstance(&pObj);
    if (SUCCEEDED(hr))
    {
        hr = pObj->QueryInterface(ppOut);
        pObj->m_frontPanel = m_frontPanels[idx].fp;
        m_frontPanels[idx].inUse = pObj;
    }
    return hr;
}

HRESULT CRc1101Manager::ConnectDevice(ULONG idx, IUnknown *pRig)
{
    if ((idx >= m_frontPanels.size()) || !pRig)
        return E_INVALIDARG;
    if (!m_frontPanels[idx].inUse)
        return E_UNEXPECTED;
   return m_frontPanels[idx].inUse->Initialize(pRig);
}

HRESULT CRc1101Manager::DisconnectDevice(ULONG idx)
{
    if (idx >= m_frontPanels.size())
        return E_INVALIDARG;

    if (!m_frontPanels[idx].inUse)
        return E_UNEXPECTED;
    auto stabilize = m_frontPanels[idx].inUse;
    stabilize->Disconnect();
    return S_OK;
}

HRESULT CRc1101Manager::GetSerialNumber(ULONG idx, BSTR *v)
{
    if (idx >= m_frontPanels.size())
        return E_INVALIDARG;
    CComBSTR b;
    b = m_frontPanels[idx].serialNumber.c_str();
    *v = b.Detach();
    return S_OK;
}

HRESULT CRc1101Manager::GetIdString(ULONG idx, BSTR *v)
{
    if (idx >= m_frontPanels.size())
        return E_INVALIDARG;
    if (m_frontPanels[idx].idString.empty())
        m_frontPanels[idx].idString = m_frontPanels[idx].fp->GetIdString();
    CComBSTR b;
    if (!m_frontPanels[idx].idString.empty())
        b = m_frontPanels[idx].idString.c_str();
    else
        b = m_frontPanels[idx].serialNumber.c_str();
    *v = b.Detach();
    return S_OK;
}

HRESULT CRc1101Manager::RebootDevice(ULONG idx)
{
    if (idx >= m_frontPanels.size())
        return E_INVALIDARG;
    return m_frontPanels[idx].fp->ResetFrontPanel() ? S_OK : E_FAIL;
}