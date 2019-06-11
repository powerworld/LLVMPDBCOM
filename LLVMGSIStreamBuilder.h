// LLVMGSIStreamBuilder.h : Declaration of the CLLVMGSIStreamBuilder

#pragma once
#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMPDBFileBuilder.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMGSIStreamBuilder

class ATL_NO_VTABLE CLLVMGSIStreamBuilder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLLVMGSIStreamBuilder, &CLSID_LLVMGSIStreamBuilder>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMGSIStreamBuilder, &IID_ILLVMGSIStreamBuilder, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<BumpPtrAllocator> bumpPtrAllocator;
	std::shared_ptr<pdb::PDBFileBuilder> pdbFileBuilder;
	boost::optional<pdb::GSIStreamBuilder&> gsiStreamBuilder;

public:
	CLLVMGSIStreamBuilder() :
		bumpPtrAllocator(nullptr), pdbFileBuilder(nullptr), gsiStreamBuilder(boost::none)
	{
	}

	void construct(const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator,
		const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder);

DECLARE_REGISTRY_RESOURCEID(115)


BEGIN_COM_MAP(CLLVMGSIStreamBuilder)
	COM_INTERFACE_ENTRY(ILLVMGSIStreamBuilder)
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
	STDMETHOD(addPublicSymbol)(VARIANT Pub);
	STDMETHOD(addGlobalSymbol)(VARIANT Sym);
};

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(LLVMGSIStreamBuilder), CLLVMGSIStreamBuilder)
