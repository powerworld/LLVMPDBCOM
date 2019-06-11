// LLVMBumpPtrAllocator.h : Declaration of the CLLVMBumpPtrAllocator

#pragma once

#include <llvm/Support/Allocator.h>
#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMBumpPtrAllocator

class ATL_NO_VTABLE CLLVMBumpPtrAllocator :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLLVMBumpPtrAllocator, &CLSID_LLVMBumpPtrAllocator>,
	public IDispatchImpl<ILLVMBumpPtrAllocator, &IID_ILLVMBumpPtrAllocator, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<BumpPtrAllocator> allocator;

public:
	CLLVMBumpPtrAllocator() :
		allocator(new BumpPtrAllocator())
	{
	}

	const std::shared_ptr<BumpPtrAllocator>& getShared()
	{
		return allocator;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMBUMPPTRALLOCATOR)

BEGIN_COM_MAP(CLLVMBumpPtrAllocator)
	COM_INTERFACE_ENTRY(ILLVMBumpPtrAllocator)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
};

OBJECT_ENTRY_AUTO(__uuidof(LLVMBumpPtrAllocator), CLLVMBumpPtrAllocator)
