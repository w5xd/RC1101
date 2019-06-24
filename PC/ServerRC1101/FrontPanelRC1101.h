#pragma once
#include "RemoteRigControl_i.h"
#include <thread>
#include <condition_variable>
#include <functional> 
#include <deque>
#include <map>

namespace RadioPanelUsb {
    class CFrontPanel;
}

class ATL_NO_VTABLE FrontPanelRC1101 :
    public ATL::CComObjectRootEx<ATL::CComMultiThreadModelNoCS>,
    public IWlControlSite
{
public:
    FrontPanelRC1101();

    DECLARE_NOT_AGGREGATABLE(FrontPanelRC1101)

    BEGIN_COM_MAP(FrontPanelRC1101)
        	COM_INTERFACE_ENTRY(IWlControlSite)
            COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
    END_COM_MAP()
	DECLARE_PROTECT_FINAL_CONSTRUCT()
    DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct();

	void FinalRelease();

    // IWlControlSite
    STDMETHOD(ContinueUpdating)();
    STDMETHOD(SetEncoderCenter)(USHORT which, LONG center);
    STDMETHOD(PressTrellisButton)(USHORT which, USHORT value);
    STDMETHOD(SetDisplayString)(USHORT which, BSTR  value);
    STDMETHOD(SetupTrellisGroup)(USHORT which, USHORT mask, USHORT objType, USHORT objIndex);
    STDMETHOD(SetEncoderMap)(USHORT which, USHORT objTypeL, USHORT objIndexL, USHORT objTypeH, USHORT objIndexH, SHORT, LONG, LONG);
    STDMETHOD(SetDisplayObjects)(USHORT count, USHORT objTypes[],
        USHORT objIndices[],
        USHORT values[]);
    STDMETHOD(ResetDisplayDefaults)();
    STDMETHOD(SetEncoderSwitchState)(USHORT);

    std::shared_ptr<RadioPanelUsb::CFrontPanel> m_frontPanel;

    HRESULT Initialize(IUnknown *pRig);
    void Disconnect();

protected:
    typedef std::unique_lock<std::mutex> lock_t;
    typedef std::function<void(FrontPanelRC1101*)> threadEntry_t;
    typedef std::function<void()> UpdateFcn_t;
    void threadEntry(IStream *);
    void addToQueue(const threadEntry_t &);

    unsigned char m_brightness;
    std::condition_variable m_cond;
    std::mutex m_mutex;
    bool m_running;
    bool m_stop;
    bool m_continueUpdating;
    bool m_haveResetDisplayDefaults;

    std::vector<unsigned>  m_encCenters;
    std::deque<threadEntry_t> m_queue;
    std::map<unsigned, threadEntry_t> m_encoderCenterQueue;
    ATL::CComPtr<IUnknown> m_pUnkMarshaler;
    std::thread m_thread;
};

