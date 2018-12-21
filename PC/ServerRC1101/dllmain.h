// dllmain.h : Declaration of module class.

class CServerRC1101Module : public ATL::CAtlDllModuleT< CServerRC1101Module >
{
public :
	DECLARE_LIBID(LIBID_ServerRC1101Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SERVERRC1101, "{30c5eb06-65f6-4d10-bbe4-48264af28655}")
};

extern class CServerRC1101Module _AtlModule;
