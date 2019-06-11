// LLVMGSIStreamBuilder.cpp : Implementation of CLLVMGSIStreamBuilder

#include "pch.h"
#include "LLVMGSIStreamBuilder.h"
#include "Utilities.h"


// CLLVMGSIStreamBuilder

void CLLVMGSIStreamBuilder::construct(const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator, 
	const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder)
{
	bumpPtrAllocator = theBumpPtrAllocator;
	pdbFileBuilder = thePDBFileBuilder;
	gsiStreamBuilder = pdbFileBuilder->getGsiBuilder();
}

STDMETHODIMP CLLVMGSIStreamBuilder::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =
	{
		&IID_ILLVMGSIStreamBuilder
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMGSIStreamBuilder::addPublicSymbol(VARIANT Pub)
{
	if (Pub.vt != (VT_ARRAY | VT_UI1)) return E_INVALIDARG;
	HRESULT hres = SafeArrayLock(Pub.parray);
	if (FAILED(hres)) return hres;
	BOOST_SCOPE_EXIT(&Pub) {
		// Unlock the array before returning
		SafeArrayUnlock(Pub.parray);
	} BOOST_SCOPE_EXIT_END
	if (SafeArrayGetDim(Pub.parray) != 1) return E_INVALIDARG;

	LONG l, u;
	hres = SafeArrayGetLBound(Pub.parray, 1, &l);
	if (FAILED(hres)) return hres;
	hres = SafeArrayGetUBound(Pub.parray, 1, &u);
	if (FAILED(hres)) return hres;

	uint32_t len = u - l + 1;
	uint8_t* storage = bumpPtrAllocator->Allocate<uint8_t>(len);
	std::memcpy(storage, Pub.parray->pvData, len);

	ArrayRef<uint8_t> rawSymbol(storage, len);
	BinaryStreamRef streamRef(rawSymbol, support::little);
	auto symbol = codeview::readSymbolFromStream(streamRef, 0);
	if (!symbol)
		return checkLLVMErrorAndSetCOMError(symbol.takeError(), GetObjectCLSID(),
			IID_ILLVMInfoStreamBuilder);

	// Only accept S_PUB32 symbols
	if (symbol.get().kind() != codeview::SymbolKind::S_PUB32) return E_INVALIDARG;
	auto pubSym = bumpPtrAllocator->Allocate<codeview::PublicSym32>(1);
	auto pubSymExp = codeview::SymbolDeserializer::deserializeAs<codeview::PublicSym32>(symbol.get());
	if (!pubSymExp)
		return checkLLVMErrorAndSetCOMError(pubSymExp.takeError(), GetObjectCLSID(), IID_ILLVMInfoStreamBuilder);
	*pubSym = pubSymExp.get(); // Copy it into permanent storage
	gsiStreamBuilder->addPublicSymbol(*pubSym);

	return S_OK;
}


STDMETHODIMP CLLVMGSIStreamBuilder::addGlobalSymbol(VARIANT Sym)
{
	if (Sym.vt != (VT_ARRAY | VT_UI1)) return E_INVALIDARG;
	HRESULT hres = SafeArrayLock(Sym.parray);
	if (FAILED(hres)) return hres;
	BOOST_SCOPE_EXIT(&Sym) {
		// Unlock the array before returning
		SafeArrayUnlock(Sym.parray);
	} BOOST_SCOPE_EXIT_END
	if (SafeArrayGetDim(Sym.parray) != 1) return E_INVALIDARG;

	LONG l, u;
	hres = SafeArrayGetLBound(Sym.parray, 1, &l);
	if (FAILED(hres)) return hres;
	hres = SafeArrayGetUBound(Sym.parray, 1, &u);
	if (FAILED(hres)) return hres;

	uint32_t len = u - l + 1;
	uint8_t* storage = bumpPtrAllocator->Allocate<uint8_t>(len);
	std::memcpy(storage, Sym.parray->pvData, len);

	ArrayRef<uint8_t> rawSymbol(storage, len);
	BinaryStreamRef streamRef(rawSymbol, support::little);
	Expected<codeview::CVSymbol> symbol = codeview::readSymbolFromStream(streamRef, 0);
	if (!symbol)
		return checkLLVMErrorAndSetCOMError(symbol.takeError(), GetObjectCLSID(), 
			IID_ILLVMInfoStreamBuilder);
	// Only accept these symbols
	switch (symbol.get().kind()) {
	case codeview::S_CONSTANT:
	case codeview::S_UDT:
	case codeview::S_GDATA32:
	case codeview::S_LDATA32:
	case codeview::S_PROCREF:
	case codeview::S_LPROCREF:
		break;
	default:
		return E_INVALIDARG;
	}
	gsiStreamBuilder->addGlobalSymbol(symbol.get());

	return S_OK;
}
