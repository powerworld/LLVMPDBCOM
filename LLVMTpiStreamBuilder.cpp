// LLVMTpiStreamBuilder.cpp : Implementation of CLLVMTpiStreamBuilder

#include "pch.h"
#include "LLVMTpiStreamBuilder.h"

// CLLVMTpiStreamBuilder

void CLLVMTpiStreamBuilder::constructTpiBuilder(const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator, 
	const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder)
{
	bumpPtrAllocator = theBumpPtrAllocator;
	pdbFileBuilder = thePDBFileBuilder;
	tpiStreamBuilder = pdbFileBuilder->getTpiBuilder();
}

void CLLVMTpiStreamBuilder::constructIpiBuilder(const std::shared_ptr<BumpPtrAllocator>& theBumpPtrAllocator, 
	const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder)
{
	bumpPtrAllocator = theBumpPtrAllocator;
	pdbFileBuilder = thePDBFileBuilder;
	tpiStreamBuilder = pdbFileBuilder->getIpiBuilder();
}

STDMETHODIMP CLLVMTpiStreamBuilder::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ILLVMTpiStreamBuilder
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMTpiStreamBuilder::setVersionHeader(LLVM_PdbRaw_TpiVer Version)
{
	tpiStreamBuilder->setVersionHeader(static_cast<pdb::PdbRaw_TpiVer>(Version));
	return S_OK;
}


STDMETHODIMP CLLVMTpiStreamBuilder::addTypeRecord(VARIANT Type, VARIANT Hash)
{
	bool hasHash = Hash.vt != VT_NULL;
	//if (hasHash && (Hash.vt != VT_UI4)) return E_INVALIDARG;
	VARIANT convHash;
	HRESULT hres;
	VariantInit(&convHash);
	BOOST_SCOPE_EXIT(&convHash) {
		VariantClear(&convHash);
	} BOOST_SCOPE_EXIT_END
	if (hasHash) {
		hres = VariantCopyInd(&convHash, &Hash);
		if (FAILED(hres)) return hres;
		hres = VariantChangeType(&convHash, &convHash, 0, VT_UI4);
		if (FAILED(hres)) return hres;
	}
	if (Type.vt != (VT_ARRAY | VT_UI1)) return E_INVALIDARG;

	hres = SafeArrayLock(Type.parray);
	if (FAILED(hres)) return hres;
	BOOST_SCOPE_EXIT(&Type) {
		SafeArrayUnlock(Type.parray);
	} BOOST_SCOPE_EXIT_END
	if (SafeArrayGetDim(Type.parray) != 1) return E_INVALIDARG;

	LONG l, u;
	hres = SafeArrayGetLBound(Type.parray, 1, &l);
	if (FAILED(hres)) return hres;
	hres = SafeArrayGetUBound(Type.parray, 1, &u);
	if (FAILED(hres)) return hres;

	uint32_t len = u - l + 1;
	uint8_t* storage = bumpPtrAllocator->Allocate<uint8_t>(len);
	std::memcpy(storage, Type.parray->pvData, len);
	if (hasHash)
		tpiStreamBuilder->addTypeRecord(ArrayRef<uint8_t>(storage, len), Hash.ulVal);
	else
		tpiStreamBuilder->addTypeRecord(ArrayRef<uint8_t>(storage, len), None);

	return S_OK;
}
