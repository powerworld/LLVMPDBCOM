// LLVMDbiModuleDescriptorBuilder.cpp : Implementation of CLLVMDbiModuleDescriptorBuilder

#include "pch.h"
#include "LLVMDbiStreamBuilder.h"
#include "LLVMDbiModuleDescriptorBuilder.h"
#include "LLVMDebugSubsection.h"
#include "Utilities.h"

using namespace boost::locale::conv;

// CLLVMDbiModuleDescriptorBuilder

HRESULT CLLVMDbiModuleDescriptorBuilder::construct(
	const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator, 
	const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder, std::string& moduleName)
{
	bumpPtrAllocator = theBumpPtrAllocator;
	pdbFileBuilder = thePDBFileBuilder;
	dbiStreamBuilder = pdbFileBuilder->getDbiBuilder();
	auto expected = dbiStreamBuilder->addModuleInfo(moduleName);
	HRESULT res = checkLLVMErrorAndSetCOMError(expected.takeError(), GetObjectCLSID(),
		IID_ILLVMDbiStreamBuilder);
	if (SUCCEEDED(res))
		dbiModuleDescriptorBuilder = *expected;
	return res;
}

STDMETHODIMP CLLVMDbiModuleDescriptorBuilder::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ILLVMDbiModuleDescriptorBuilder
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMDbiModuleDescriptorBuilder::setObjFileName(BSTR Name)
{
	dbiModuleDescriptorBuilder->setObjFileName(boost::locale::conv::utf_to_utf<char>(Name));
	return S_OK;
}

STDMETHODIMP CLLVMDbiModuleDescriptorBuilder::addSymbolsInBulk(VARIANT BulkSymbols)
{
	if (BulkSymbols.vt != (VT_ARRAY | VT_UI1)) return E_INVALIDARG;
	HRESULT hres = SafeArrayLock(BulkSymbols.parray);
	if (FAILED(hres)) return hres;
	BOOST_SCOPE_EXIT(&BulkSymbols) {
		SafeArrayUnlock(BulkSymbols.parray);
	} BOOST_SCOPE_EXIT_END
	if (SafeArrayGetDim(BulkSymbols.parray) != 1) return E_INVALIDARG;

	LONG l, u;
	hres = SafeArrayGetLBound(BulkSymbols.parray, 1, &l);
	if (FAILED(hres)) return hres;
	hres = SafeArrayGetUBound(BulkSymbols.parray, 1, &u);
	if (FAILED(hres)) return hres;
	
	uint32_t len = u - l + 1;
	uint8_t* storage = bumpPtrAllocator->Allocate<uint8_t>(len);
	std::memcpy(storage, BulkSymbols.parray->pvData, len);
	dbiModuleDescriptorBuilder->addSymbolsInBulk(
		ArrayRef<uint8_t>(storage, len));
	
	return S_OK;
}


STDMETHODIMP CLLVMDbiModuleDescriptorBuilder::getObjFileName(BSTR* res)
{
	CComBSTR temp(
		utf_to_utf<wchar_t>(
			dbiModuleDescriptorBuilder->getObjFileName().str())
		.c_str());
	return temp.CopyTo(res);
}


STDMETHODIMP CLLVMDbiModuleDescriptorBuilder::addDebugSubsection(ILLVMDebugSubsection* Subsection)
{
	dbiModuleDescriptorBuilder->addDebugSubsection(
		dynamic_cast<CLLVMDebugSubsection*>(Subsection)->getSharedDebugSubsection());
	return S_OK;
}


STDMETHODIMP CLLVMDbiModuleDescriptorBuilder::setFirstSectionContrib(const LLVM_SectionContrib* SC)
{
	dbiModuleDescriptorBuilder->setFirstSectionContrib(*reinterpret_cast<const pdb::SectionContrib*>(SC));
	return S_OK;
}


STDMETHODIMP CLLVMDbiModuleDescriptorBuilder::getModuleIndex(ULONG* ModI)
{
	*ModI = dbiModuleDescriptorBuilder->getModuleIndex();
	return S_OK;
}


STDMETHODIMP CLLVMDbiModuleDescriptorBuilder::setPdbFilePathNI(ULONG NI)
{
	dbiModuleDescriptorBuilder->setPdbFilePathNI(NI);
	return S_OK;
}


STDMETHODIMP CLLVMDbiModuleDescriptorBuilder::addSymbol(VARIANT Symbol)
{
	if (Symbol.vt != (VT_ARRAY | VT_UI1)) return E_INVALIDARG;
	HRESULT hres = SafeArrayLock(Symbol.parray);
	if (FAILED(hres)) return hres;
	BOOST_SCOPE_EXIT(&Symbol) {
		SafeArrayUnlock(Symbol.parray);
	} BOOST_SCOPE_EXIT_END
	if (SafeArrayGetDim(Symbol.parray) != 1) return E_INVALIDARG;

	LONG l, u;
	hres = SafeArrayGetLBound(Symbol.parray, 1, &l);
	if (FAILED(hres)) return hres;
	hres = SafeArrayGetUBound(Symbol.parray, 1, &u);
	if (FAILED(hres)) return hres;

	uint32_t len = u - l + 1;
	uint8_t* storage = reinterpret_cast<uint8_t*>(bumpPtrAllocator->Allocate(len, 4));
	std::memcpy(storage, Symbol.parray->pvData, len);

	// Technically not necessary, but it does provide a nice sanity check that the symbol coming in
	// is valid
	ArrayRef<uint8_t> rawSymbol(storage, len);
	BinaryStreamRef streamRef(rawSymbol, support::little);
	Expected<codeview::CVSymbol> symbol = codeview::readSymbolFromStream(streamRef, 0);
	if (!symbol)
		return checkLLVMErrorAndSetCOMError(symbol.takeError(), GetObjectCLSID(),
			IID_ILLVMDbiModuleDescriptorBuilder);

	dbiModuleDescriptorBuilder->addSymbol(symbol.get());

	return S_OK;
}
