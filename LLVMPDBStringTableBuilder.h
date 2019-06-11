// LLVMPDBStringTableBuilder.h : Declaration of the CLLVMPDBStringTableBuilder

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMPDBFileBuilder.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMPDBStringTableBuilder

class ATL_NO_VTABLE CLLVMPDBStringTableBuilder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLLVMPDBStringTableBuilder, &CLSID_LLVMPDBStringTableBuilder>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMPDBStringTableBuilder, &IID_ILLVMPDBStringTableBuilder, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<pdb::PDBFileBuilder> pdbFileBuilder;
	boost::optional<pdb::PDBStringTableBuilder&> pdbStringTableBuilder;

public:
	CLLVMPDBStringTableBuilder() :
		pdbFileBuilder(nullptr), pdbStringTableBuilder(boost::none)
	{
	}

	void construct(const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder);

DECLARE_REGISTRY_RESOURCEID(IDR_LLVMPDBSTRINGTABLEBUILDER)


BEGIN_COM_MAP(CLLVMPDBStringTableBuilder)
	COM_INTERFACE_ENTRY(ILLVMPDBStringTableBuilder)
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
	STDMETHOD(insert)(BSTR S, ULONG* Res);
	STDMETHOD(getIdForString)(BSTR S, ULONG* Res);
	STDMETHOD(getStringForId)(ULONG Id, BSTR* Res);
	STDMETHOD(setStrings)(ILLVMDebugStringTableSubsection* Strings);
};

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(LLVMPDBStringTableBuilder), CLLVMPDBStringTableBuilder)
