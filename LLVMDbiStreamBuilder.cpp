// LLVMDbiStreamBuilder.cpp : Implementation of CLLVMDbiStreamBuilder

#include "pch.h"
#include "LLVMDbiStreamBuilder.h"
#include "LLVMDbiModuleDescriptorBuilder.h"
#include "Utilities.h"

// CLLVMDbiStreamBuilder

using namespace boost::locale::conv;

void CLLVMDbiStreamBuilder::construct(const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator, 
	const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder)
{
	bumpPtrAllocator = theBumpPtrAllocator;
	pdbFileBuilder = thePDBFileBuilder;
	dbiStreamBuilder = pdbFileBuilder->getDbiBuilder();
}

STDMETHODIMP CLLVMDbiStreamBuilder::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ILLVMDbiStreamBuilder
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setVersionHeader(LLVM_PdbRaw_DbiVer V)
{
	dbiStreamBuilder->setVersionHeader(static_cast<pdb::PdbRaw_DbiVer>(V));
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setAge(ULONG A)
{
	dbiStreamBuilder->setAge(A);
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setBuildNumber(USHORT B)
{
	dbiStreamBuilder->setBuildNumber(B);
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setBuildNumberMajorMinor(BYTE Major, BYTE Minor)
{
	dbiStreamBuilder->setBuildNumber(Major, Minor);
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setPdbDllVersion(USHORT V)
{
	dbiStreamBuilder->setPdbDllVersion(V);
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setPdbDllRbld(USHORT R)
{
	dbiStreamBuilder->setPdbDllRbld(R);
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setFlags(USHORT F)
{
	dbiStreamBuilder->setFlags(F);
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setMachineType(LLVM_PDB_Machine M)
{
	dbiStreamBuilder->setMachineType(static_cast<pdb::PDB_Machine>(M));
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setMachineTypeCOFF(ULONG M)
{
	dbiStreamBuilder->setMachineType(static_cast<COFF::MachineTypes>(M));
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::addModuleInfo(BSTR ModuleName, ILLVMDbiModuleDescriptorBuilder** builder)
{
	CComObject<CLLVMDbiModuleDescriptorBuilder>* tempBuilder;
	CComObject<CLLVMDbiModuleDescriptorBuilder>::CreateInstance(&tempBuilder);
	tempBuilder->AddRef();
	HRESULT res = tempBuilder->construct(bumpPtrAllocator, pdbFileBuilder, utf_to_utf<char>(ModuleName));
	if (SUCCEEDED(res))
	{
		*builder = tempBuilder;
		return S_OK;
	}
	else
	{
		tempBuilder->Release();
		*builder = nullptr;
		return res;
	}
}


STDMETHODIMP CLLVMDbiStreamBuilder::addModuleSourceFile(ILLVMDbiModuleDescriptorBuilder* Module, BSTR File)
{
	CComPtr<ILLVMDbiModuleDescriptorBuilder> pModule(Module);
	return checkLLVMErrorAndSetCOMError(
		dbiStreamBuilder->addModuleSourceFile(
			static_cast<CLLVMDbiModuleDescriptorBuilder*>(pModule.p)->getNative(),
			utf_to_utf<char>(File)),
		GetObjectCLSID(),
		IID_ILLVMDbiStreamBuilder);
}


STDMETHODIMP CLLVMDbiStreamBuilder::addECName(BSTR Name, ULONG* Res)
{
	*Res = dbiStreamBuilder->addECName(utf_to_utf<char>(Name));
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::addSectionContrib(const LLVM_SectionContrib* SC)
{
	dbiStreamBuilder->addSectionContrib(*reinterpret_cast<const pdb::SectionContrib*>(SC));
	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::setSectionMap(VARIANT SecMap)
{
	if (SecMap.vt != (VT_ARRAY | VT_UI1)) return E_INVALIDARG;
	HRESULT hres = SafeArrayLock(SecMap.parray);
	if (FAILED(hres)) return hres;
	BOOST_SCOPE_EXIT(&SecMap) {
		SafeArrayUnlock(SecMap.parray);
	} BOOST_SCOPE_EXIT_END
	if (SafeArrayGetDim(SecMap.parray) != 1) return E_INVALIDARG;

	LONG l, u;
	hres = SafeArrayGetLBound(SecMap.parray, 1, &l);
	if (FAILED(hres)) return hres;
	hres = SafeArrayGetUBound(SecMap.parray, 1, &u);
	if (FAILED(hres)) return hres;

	ArrayRef<object::coff_section> Sections(
		reinterpret_cast<const object::coff_section*>(SecMap.parray->pvData),
		(u - l + 1) / sizeof(object::coff_section));
	localSectionMap = pdb::DbiStreamBuilder::createSectionMap(Sections);
	dbiStreamBuilder->setSectionMap(localSectionMap);

	return S_OK;
}


STDMETHODIMP CLLVMDbiStreamBuilder::addDbgStream(LLVM_DbgHeaderType Type, VARIANT Data)
{
	if (Data.vt != (VT_ARRAY | VT_UI1)) return E_INVALIDARG;
	HRESULT hres = SafeArrayLock(Data.parray);
	if (FAILED(hres)) return hres;
	BOOST_SCOPE_EXIT(&Data) {
		SafeArrayUnlock(Data.parray);
	} BOOST_SCOPE_EXIT_END
	if (SafeArrayGetDim(Data.parray) != 1) return E_INVALIDARG;

	LONG l, u;
	hres = SafeArrayGetLBound(Data.parray, 1, &l);
	if (FAILED(hres)) return hres;
	hres = SafeArrayGetUBound(Data.parray, 1, &u);
	if (FAILED(hres)) return hres;

	uint32_t len = u - l + 1;
	uint8_t* storage = bumpPtrAllocator->Allocate<uint8_t>(len);
	std::memcpy(storage, Data.parray->pvData, len);

	return checkLLVMErrorAndSetCOMError(
		dbiStreamBuilder->addDbgStream(static_cast<pdb::DbgHeaderType>(Type),
			ArrayRef<uint8_t>(storage, len)),
		GetObjectCLSID(), IID_ILLVMDbiStreamBuilder);
}
