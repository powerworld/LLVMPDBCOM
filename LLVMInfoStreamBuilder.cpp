// LLVMInfoStreamBuilder.cpp : Implementation of CLLVMInfoStreamBuilder

#include "pch.h"
#include "LLVMInfoStreamBuilder.h"
#include "Utilities.h"


// CLLVMInfoStreamBuilder

void CLLVMInfoStreamBuilder::construct(const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder)
{
	pdbFileBuilder = thePDBFileBuilder;
	infoStreamBuilder = pdbFileBuilder->getInfoBuilder();
}


STDMETHODIMP CLLVMInfoStreamBuilder::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =
	{
		&IID_ILLVMInfoStreamBuilder
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMInfoStreamBuilder::setVersion(LLVM_PdbRaw_ImplVer V)
{
	infoStreamBuilder->setVersion(static_cast<pdb::PdbRaw_ImplVer>(V));
	return S_OK;
}


STDMETHODIMP CLLVMInfoStreamBuilder::addFeature(LLVM_PdbRaw_FeatureSig Sig)
{
	infoStreamBuilder->addFeature(static_cast<pdb::PdbRaw_FeatureSig>(Sig));
	return S_OK;
}


STDMETHODIMP CLLVMInfoStreamBuilder::setHashPDBContentsToGUID(VARIANT_BOOL B)
{
	infoStreamBuilder->setHashPDBContentsToGUID(B != VARIANT_FALSE);
	return S_OK;
}


STDMETHODIMP CLLVMInfoStreamBuilder::setSignature(ULONG S)
{
	infoStreamBuilder->setSignature(S);
	return S_OK;
}


STDMETHODIMP CLLVMInfoStreamBuilder::setAge(ULONG A)
{
	infoStreamBuilder->setAge(A);
	return S_OK;
}


STDMETHODIMP CLLVMInfoStreamBuilder::setGuid(GUID G)
{
	infoStreamBuilder->setGuid(*reinterpret_cast<llvm::codeview::GUID*>(&G));
	return S_OK;
}


STDMETHODIMP CLLVMInfoStreamBuilder::getAge(ULONG* age)
{
	infoStreamBuilder->getAge();
	return S_OK;
}


STDMETHODIMP CLLVMInfoStreamBuilder::getGuid(GUID* guid)
{
	*guid = *reinterpret_cast<GUID*>(&infoStreamBuilder->getGuid());
	return S_OK;
}


STDMETHODIMP CLLVMInfoStreamBuilder::getSignature(VARIANT_BOOL* hasSignature, ULONG* signature)
{
	*hasSignature = infoStreamBuilder->getSignature().hasValue();
	if (*hasSignature)
		*signature = infoStreamBuilder->getSignature().getValue();
	return S_OK;
}


STDMETHODIMP CLLVMInfoStreamBuilder::finalizeMsfLayout()
{
	return checkLLVMErrorAndSetCOMError(infoStreamBuilder->finalizeMsfLayout(),
		GetObjectCLSID(), IID_ILLVMInfoStreamBuilder);
}
