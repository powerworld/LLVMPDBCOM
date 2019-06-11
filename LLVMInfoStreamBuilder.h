// LLVMInfoStreamBuilder.h : Declaration of the CLLVMInfoStreamBuilder

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMPDBFileBuilder.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMInfoStreamBuilder

class ATL_NO_VTABLE CLLVMInfoStreamBuilder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLLVMInfoStreamBuilder, &CLSID_LLVMInfoStreamBuilder>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMInfoStreamBuilder, &IID_ILLVMInfoStreamBuilder, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<pdb::PDBFileBuilder> pdbFileBuilder;
	boost::optional<pdb::InfoStreamBuilder&> infoStreamBuilder;

public:
	CLLVMInfoStreamBuilder() :
		pdbFileBuilder(nullptr), infoStreamBuilder(boost::none)
	{
	}

	void construct(const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder);

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMINFOSTREAMBUILDER)


BEGIN_COM_MAP(CLLVMInfoStreamBuilder)
	COM_INTERFACE_ENTRY(ILLVMInfoStreamBuilder)
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
	STDMETHOD(setVersion)(LLVM_PdbRaw_ImplVer V);
	STDMETHOD(addFeature)(LLVM_PdbRaw_FeatureSig Sig);
	STDMETHOD(setHashPDBContentsToGUID)(VARIANT_BOOL B);
	STDMETHOD(setSignature)(ULONG S);
	STDMETHOD(setAge)(ULONG A);
	STDMETHOD(setGuid)(GUID G);
	STDMETHOD(getAge)(ULONG* age);
	STDMETHOD(getGuid)(GUID* guid);
	STDMETHOD(getSignature)(VARIANT_BOOL* hasSignature, ULONG* signature);
	STDMETHOD(finalizeMsfLayout)();
};

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(LLVMInfoStreamBuilder), CLLVMInfoStreamBuilder)
