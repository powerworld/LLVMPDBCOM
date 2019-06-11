// LLVMDbiModuleDescriptorBuilder.h : Declaration of the CLLVMDbiModuleDescriptorBuilder

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMDbiModuleDescriptorBuilder

class ATL_NO_VTABLE CLLVMDbiModuleDescriptorBuilder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLLVMDbiModuleDescriptorBuilder, &CLSID_LLVMDbiModuleDescriptorBuilder>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMDbiModuleDescriptorBuilder, &IID_ILLVMDbiModuleDescriptorBuilder, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<pdb::PDBFileBuilder> pdbFileBuilder;
	std::shared_ptr<BumpPtrAllocator> bumpPtrAllocator;
	boost::optional<pdb::DbiStreamBuilder&> dbiStreamBuilder;
	boost::optional<pdb::DbiModuleDescriptorBuilder&> dbiModuleDescriptorBuilder;

public:
	CLLVMDbiModuleDescriptorBuilder() :
		pdbFileBuilder(nullptr), bumpPtrAllocator(nullptr), dbiStreamBuilder(boost::none),
		dbiModuleDescriptorBuilder(boost::none)
	{
	}

	pdb::DbiModuleDescriptorBuilder& getNative()
	{
		return *dbiModuleDescriptorBuilder;
	}

	HRESULT construct(const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator, 
		const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder, std::string& moduleName);

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMDBIMODULEDESCRIPTORBUILDER)


BEGIN_COM_MAP(CLLVMDbiModuleDescriptorBuilder)
	COM_INTERFACE_ENTRY(ILLVMDbiModuleDescriptorBuilder)
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
	STDMETHOD(setObjFileName)(BSTR Name);
	STDMETHOD(addSymbolsInBulk)(VARIANT BulkSymbols);
	STDMETHOD(getObjFileName)(BSTR* res);
	STDMETHOD(addDebugSubsection)(ILLVMDebugSubsection* Subsection);
	STDMETHOD(setFirstSectionContrib)(const LLVM_SectionContrib* SC);
	STDMETHOD(getModuleIndex)(ULONG* ModI);
	STDMETHOD(setPdbFilePathNI)(ULONG NI);
	STDMETHOD(addSymbol)(VARIANT Symbol);
};

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(LLVMDbiModuleDescriptorBuilder), CLLVMDbiModuleDescriptorBuilder)
