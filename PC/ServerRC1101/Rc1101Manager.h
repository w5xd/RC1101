// Rc1101Manager.h : Declaration of the CRc1101Manager

#pragma once
#include "resource.h"       // main symbols
#include "ServerRC1101_i.h"
#include "FrontPanelRC1101.h"

using namespace ATL;

namespace RadioPanelUsb {
    class CFrontPanel;
}

// CRc1101Manager

class ATL_NO_VTABLE CRc1101Manager :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CRc1101Manager, &CLSID_Rc1101Manager>,
    public IDispatchImpl<IRc1101Manager, &IID_IRc1101Manager, &LIBID_ServerRC1101Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CRc1101Manager();

    DECLARE_REGISTRY_RESOURCEID(106)

    DECLARE_NOT_AGGREGATABLE(CRc1101Manager)

    BEGIN_COM_MAP(CRc1101Manager)
        COM_INTERFACE_ENTRY(IRc1101Manager)
        COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct();

    void FinalRelease();
public:
    // IRc1101Manager
    STDMETHOD(ListDevices)(ULONG *pVal);
    STDMETHOD(OpenByUsbSerialNumber)(BSTR sn, ULONG *pIndex);
    STDMETHOD(ResetAndOpenDevice)(BSTR sn, ULONG *pIndex);
    STDMETHOD(GetDevice)( ULONG idx, IUnknown **ppOut);
    STDMETHOD(ConnectDevice)(ULONG idx, IUnknown *pRig);
    STDMETHOD(DisconnectDevice)(ULONG idx);
    STDMETHOD(GetSerialNumber)(ULONG idx, BSTR *);
    STDMETHOD(GetIdString)(ULONG idx, BSTR*);
    STDMETHOD(RebootDevice)(ULONG idx);

protected:
    struct FrontPanel {
        FrontPanel()
        {}
        FrontPanel(const std::string &sn, std::shared_ptr<RadioPanelUsb::CFrontPanel> f )
            : fp(f), serialNumber(sn) {};
        std::shared_ptr<RadioPanelUsb::CFrontPanel> fp;
        std::string serialNumber;
        std::string idString;
        CComPtr<FrontPanelRC1101> inUse;
    };
    std::vector<FrontPanel> m_frontPanels;
};

OBJECT_ENTRY_AUTO(__uuidof(Rc1101Manager), CRc1101Manager)
