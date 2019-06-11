// LLVMDebugStringTableSubsection.h : Declaration of the CLLVMDebugStringTableSubsection

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMDebugSubsection.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMDebugStringTableSubsection

class ATL_NO_VTABLE CLLVMDebugStringTableSubsection :
	public CLLVMDebugSubsection,
	public CComCoClass<CLLVMDebugStringTableSubsection, &CLSID_LLVMDebugStringTableSubsection>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMDebugStringTableSubsection, &IID_ILLVMDebugStringTableSubsection, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<codeview::DebugStringTableSubsection> debugStringTableSubsection;
	std::shared_ptr<codeview::DebugSubsection> debugSubsection;

public:
	CLLVMDebugStringTableSubsection() :
		debugStringTableSubsection(new codeview::DebugStringTableSubsection()), debugSubsection(debugStringTableSubsection)
	{
	}

	const std::shared_ptr<codeview::DebugStringTableSubsection>& getShared()
	{
		return debugStringTableSubsection;
	}

	const std::shared_ptr<codeview::DebugSubsection>& getSharedDebugSubsection() override
	{
		return debugSubsection;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMDEBUGSTRINGTABLESUBSECTION)

BEGIN_COM_MAP(CLLVMDebugStringTableSubsection)
	COM_INTERFACE_ENTRY(ILLVMDebugStringTableSubsection)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(insert)(BSTR S, ULONG* Res);
	STDMETHOD(getIdForString)(BSTR S, ULONG* Res);
	STDMETHOD(getStringForId)(ULONG Id, BSTR* Res);
};

OBJECT_ENTRY_AUTO(__uuidof(LLVMDebugStringTableSubsection), CLLVMDebugStringTableSubsection)
