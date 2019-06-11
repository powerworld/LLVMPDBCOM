// dllmain.h : Declaration of module class.

class CLLVMCOMModule : public ATL::CAtlDllModuleT< CLLVMCOMModule >
{
public :
	DECLARE_LIBID(LIBID_LLVMPDBCOM)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LLVMCOM, "{1ad0d8aa-7e3a-40b0-b209-05a677af6131}")
};

extern class CLLVMCOMModule _AtlModule;
