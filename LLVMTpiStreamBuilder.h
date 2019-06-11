// LLVMTpiStreamBuilder.h : Declaration of the CLLVMTpiStreamBuilder

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMPDBFileBuilder.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMTpiStreamBuilder

class ATL_NO_VTABLE CLLVMTpiStreamBuilder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLLVMTpiStreamBuilder, &CLSID_LLVMTpiStreamBuilder>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMTpiStreamBuilder, &IID_ILLVMTpiStreamBuilder, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<BumpPtrAllocator> bumpPtrAllocator;
	std::shared_ptr<pdb::PDBFileBuilder> pdbFileBuilder;
	boost::optional<pdb::TpiStreamBuilder&> tpiStreamBuilder;

public:
	CLLVMTpiStreamBuilder() :
		bumpPtrAllocator(nullptr), pdbFileBuilder(nullptr), tpiStreamBuilder(boost::none)
	{
	}

	void constructTpiBuilder(const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator, 
		const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder);
	void constructIpiBuilder(const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator, 
		const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder);

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMTPISTREAMBUILDER)


BEGIN_COM_MAP(CLLVMTpiStreamBuilder)
	COM_INTERFACE_ENTRY(ILLVMTpiStreamBuilder)
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



	STDMETHOD(setVersionHeader)(LLVM_PdbRaw_TpiVer Version);
	STDMETHOD(addTypeRecord)(VARIANT Type, VARIANT Hash);
};

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(LLVMTpiStreamBuilder), CLLVMTpiStreamBuilder)
