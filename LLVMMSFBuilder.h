// LLVMMSFBuilder.h : Declaration of the CLLVMMSFBuilder

#pragma once

#include "resource.h"       // main symbols
#include "LLVMPDBCOM_i.h"
#include "LLVMPDBFileBuilder.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace llvm;

// CLLVMMSFBuilder

class ATL_NO_VTABLE CLLVMMSFBuilder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLLVMMSFBuilder, &CLSID_LLVMMSFBuilder>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILLVMMSFBuilder, &IID_ILLVMMSFBuilder, &LIBID_LLVMPDBCOM, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	std::shared_ptr<pdb::PDBFileBuilder> pdbFileBuilder;
	boost::optional<msf::MSFBuilder&> msfBuilder;

public:
	CLLVMMSFBuilder() :
		pdbFileBuilder(nullptr), msfBuilder(boost::none)
	{
	}

	void construct(const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder);

DECLARE_REGISTRY_RESOURCEID(113)


BEGIN_COM_MAP(CLLVMMSFBuilder)
	COM_INTERFACE_ENTRY(ILLVMMSFBuilder)
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
	STDMETHOD(addStream)(ULONG Size, ULONG* Res);
};

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(LLVMMSFBuilder), CLLVMMSFBuilder)
