// LLVMDebugLinesSubsection.h : Declaration of the CLLVMDebugLinesSubsection

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMDebugChecksumsSubsection.h"
#include "LLVMDebugStringTableSubsection.h"
#include "LLVMDebugSubsection.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMDebugLinesSubsection

class ATL_NO_VTABLE CLLVMDebugLinesSubsection :
	public CLLVMDebugSubsection,
	public CComCoClass<CLLVMDebugLinesSubsection, &CLSID_LLVMDebugLinesSubsection>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMDebugLinesSubsection, &IID_ILLVMDebugLinesSubsection, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<codeview::DebugChecksumsSubsection> debugChecksumsSubsection;
	std::shared_ptr<codeview::DebugStringTableSubsection> debugStringTableSubsection;
	std::shared_ptr<codeview::DebugLinesSubsection> debugLinesSubsection;
	std::shared_ptr<codeview::DebugSubsection> debugSubsection;

public:
	CLLVMDebugLinesSubsection() :
		debugChecksumsSubsection(nullptr), debugStringTableSubsection(nullptr),
		debugLinesSubsection(nullptr), debugSubsection(nullptr)
	{
	}

	const std::shared_ptr<codeview::DebugSubsection>& getSharedDebugSubsection() override
	{
		return debugSubsection;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMDEBUGLINESSUBSECTION)


BEGIN_COM_MAP(CLLVMDebugLinesSubsection)
	COM_INTERFACE_ENTRY(ILLVMDebugLinesSubsection)
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
	STDMETHOD(construct)(ILLVMDebugChecksumsSubsection* Checksums, ILLVMDebugStringTableSubsection* Strings);
	STDMETHOD(setRelocationAddress)(USHORT Segment, ULONG Offset);
	STDMETHOD(setCodeSize)(ULONG Size);
	STDMETHOD(createBlock)(BSTR FileName);
	STDMETHOD(addLineInfo)(ULONG Offset, ULONG StartLine, ULONG EndLine, VARIANT_BOOL IsStatement);
};

OBJECT_ENTRY_AUTO(__uuidof(LLVMDebugLinesSubsection), CLLVMDebugLinesSubsection)
