// LLVMDbiStreamBuilder.h : Declaration of the CLLVMDbiStreamBuilder

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMPDBFileBuilder.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMDbiStreamBuilder

class ATL_NO_VTABLE CLLVMDbiStreamBuilder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLLVMDbiStreamBuilder, &CLSID_LLVMDbiStreamBuilder>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMDbiStreamBuilder, &IID_ILLVMDbiStreamBuilder, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<BumpPtrAllocator> bumpPtrAllocator;
	std::shared_ptr<pdb::PDBFileBuilder> pdbFileBuilder;
	boost::optional<pdb::DbiStreamBuilder&> dbiStreamBuilder;
	std::vector<pdb::SecMapEntry> localSectionMap;

public:
	CLLVMDbiStreamBuilder() :
		pdbFileBuilder(nullptr), dbiStreamBuilder(boost::none), localSectionMap()
	{
	}

	void construct(const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator, 
		const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder);

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMDBISTREAMBUILDER)


BEGIN_COM_MAP(CLLVMDbiStreamBuilder)
	COM_INTERFACE_ENTRY(ILLVMDbiStreamBuilder)
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
	STDMETHOD(setVersionHeader)(LLVM_PdbRaw_DbiVer V);
	STDMETHOD(setAge)(ULONG A);
	STDMETHOD(setBuildNumber)(USHORT B);
	STDMETHOD(setBuildNumberMajorMinor)(BYTE Major, BYTE Minor);
	STDMETHOD(setPdbDllVersion)(USHORT V);
	STDMETHOD(setPdbDllRbld)(USHORT R);
	STDMETHOD(setFlags)(USHORT F);
	STDMETHOD(setMachineType)(LLVM_PDB_Machine M);
	STDMETHOD(setMachineTypeCOFF)(ULONG M);
	STDMETHOD(addModuleInfo)(BSTR ModuleName, ILLVMDbiModuleDescriptorBuilder** builder);
	STDMETHOD(addModuleSourceFile)(ILLVMDbiModuleDescriptorBuilder* Module, BSTR File);
	STDMETHOD(addECName)(BSTR Name, ULONG* Res);
	STDMETHOD(addSectionContrib)(const LLVM_SectionContrib* SC);
	STDMETHOD(setSectionMap)(VARIANT SecMap);
	STDMETHOD(addDbgStream)(LLVM_DbgHeaderType Type, VARIANT Data);
};

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(LLVMDbiStreamBuilder), CLLVMDbiStreamBuilder)
