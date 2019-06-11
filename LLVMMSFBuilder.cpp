// LLVMMSFBuilder.cpp : Implementation of CLLVMMSFBuilder

#include "pch.h"
#include "LLVMMSFBuilder.h"
#include "Utilities.h"

// CLLVMMSFBuilder

void CLLVMMSFBuilder::construct(const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder)
{
	pdbFileBuilder = thePDBFileBuilder;
	msfBuilder = pdbFileBuilder->getMsfBuilder();
}

STDMETHODIMP CLLVMMSFBuilder::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ILLVMMSFBuilder
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMMSFBuilder::addStream(ULONG Size, ULONG* Res)
{
	auto expected = msfBuilder->addStream(Size);
	HRESULT hres = checkLLVMErrorAndSetCOMError(expected.takeError(), GetObjectCLSID(),
		IID_ILLVMMSFBuilder);
	if (SUCCEEDED(hres))
		*Res = *expected;
	return hres;
}
