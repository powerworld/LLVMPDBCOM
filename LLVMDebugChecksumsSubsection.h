// LLVMDebugChecksumsSubsection.h : Declaration of the CLLVMDebugChecksumsSubsection

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMDebugSubsection.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMDebugChecksumsSubsection

class ATL_NO_VTABLE CLLVMDebugChecksumsSubsection :
	public CLLVMDebugSubsection,
	public CComCoClass<CLLVMDebugChecksumsSubsection, &CLSID_LLVMDebugChecksumsSubsection>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMDebugChecksumsSubsection, &IID_ILLVMDebugChecksumsSubsection, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<BumpPtrAllocator> bumpPtrAllocator;
	std::shared_ptr<codeview::DebugStringTableSubsection> debugStringTableSubsection;
	std::shared_ptr<codeview::DebugChecksumsSubsection> debugChecksumsSubsection;
	std::shared_ptr<codeview::DebugSubsection> debugSubsection;

public:
	CLLVMDebugChecksumsSubsection() :
		bumpPtrAllocator(nullptr), debugStringTableSubsection(nullptr),
		debugChecksumsSubsection(nullptr), debugSubsection(nullptr)
	{
	}

	const std::shared_ptr<codeview::DebugChecksumsSubsection>& getShared()
	{
		return debugChecksumsSubsection;
	}

	const std::shared_ptr<codeview::DebugSubsection>& getSharedDebugSubsection() override
	{
		return debugSubsection;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMDEBUGCHECKSUMSSUBSECTION)


BEGIN_COM_MAP(CLLVMDebugChecksumsSubsection)
	COM_INTERFACE_ENTRY(ILLVMDebugChecksumsSubsection)
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
	STDMETHOD(construct)(ILLVMBumpPtrAllocator* Allocator, ILLVMDebugStringTableSubsection* Strings);
	STDMETHOD(addChecksum)(BSTR FileName, LLVM_FileChecksumKind Kind, VARIANT Bytes);
};

OBJECT_ENTRY_AUTO(__uuidof(LLVMDebugChecksumsSubsection), CLLVMDebugChecksumsSubsection)
