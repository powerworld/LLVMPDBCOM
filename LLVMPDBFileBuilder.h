// LLVMPDBFileBuilder.h : Declaration of the CLLVMPDBFileBuilder

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMBumpPtrAllocator.h"
#include "LLVMMSFBuilder.h"
#include "LLVMInfoStreamBuilder.h"
#include "LLVMDbiStreamBuilder.h"
#include "LLVMTpiStreamBuilder.h"
#include "LLVMGSIStreamBuilder.h"
#include "LLVMPDBStringTableBuilder.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMPDBFileBuilder
class ATL_NO_VTABLE CLLVMPDBFileBuilder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLLVMPDBFileBuilder, &CLSID_LLVMPDBFileBuilder>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMPDBFileBuilder, &IID_ILLVMPDBFileBuilder, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<BumpPtrAllocator> bumpPtrAllocator;
	std::shared_ptr<pdb::PDBFileBuilder> pdbFileBuilder;

public:
	CLLVMPDBFileBuilder() :
		bumpPtrAllocator(nullptr), pdbFileBuilder(nullptr)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMPDBFILEBUILDER)

BEGIN_COM_MAP(CLLVMPDBFileBuilder)
	COM_INTERFACE_ENTRY(ILLVMPDBFileBuilder)
	COM_INTERFACE_ENTRY(IDispatch)
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
	STDMETHOD(construct)(ILLVMBumpPtrAllocator* allocator);
	STDMETHOD(initialize)(ULONG blockSize);
	STDMETHOD(getMsfBuilder)(ILLVMMSFBuilder** builder);
	STDMETHOD(getInfoBuilder)(ILLVMInfoStreamBuilder** builder);
	STDMETHOD(getDbiBuilder)(ILLVMDbiStreamBuilder** builder);
	STDMETHOD(getTpiBuilder)(ILLVMTpiStreamBuilder** builder);
	STDMETHOD(getIpiBuilder)(ILLVMTpiStreamBuilder** builder);
	STDMETHOD(getGsiBuilder)(ILLVMGSIStreamBuilder** builder);
	STDMETHOD(getStringTableBuilder)(ILLVMPDBStringTableBuilder** builder);
	STDMETHOD(commit)(BSTR Filename, GUID* Guid);
};

OBJECT_ENTRY_AUTO(__uuidof(LLVMPDBFileBuilder), CLLVMPDBFileBuilder)
