// LLVMPDBFileBuilder.cpp : Implementation of CLLVMPDBFileBuilder

#include "pch.h"
#include "LLVMPDBFileBuilder.h"
#include "Utilities.h"

using namespace boost::locale::conv;

// CLLVMPDBFileBuilder

STDMETHODIMP CLLVMPDBFileBuilder::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =
	{
		&IID_ILLVMPDBFileBuilder
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMPDBFileBuilder::construct(ILLVMBumpPtrAllocator* allocator)
{
	CComPtr<ILLVMBumpPtrAllocator> pAllocator(allocator);
	bumpPtrAllocator = static_cast<CLLVMBumpPtrAllocator*>(pAllocator.p)->getShared();
	pdbFileBuilder = std::make_unique<pdb::PDBFileBuilder>(*bumpPtrAllocator);
	return S_OK;
}


STDMETHODIMP CLLVMPDBFileBuilder::initialize(ULONG blockSize)
{
	return checkLLVMErrorAndSetCOMError(pdbFileBuilder->initialize(blockSize), GetObjectCLSID(), IID_ILLVMPDBFileBuilder);
}


STDMETHODIMP CLLVMPDBFileBuilder::getMsfBuilder(ILLVMMSFBuilder** builder)
{
	CComObject<CLLVMMSFBuilder>* tempBuilder;
	CComObject<CLLVMMSFBuilder>::CreateInstance(&tempBuilder);
	tempBuilder->AddRef();
	tempBuilder->construct(pdbFileBuilder);
	*builder = tempBuilder;
	return S_OK;
}


STDMETHODIMP CLLVMPDBFileBuilder::getInfoBuilder(ILLVMInfoStreamBuilder** builder)
{
	CComObject<CLLVMInfoStreamBuilder>* tempBuilder;
	CComObject<CLLVMInfoStreamBuilder>::CreateInstance(&tempBuilder);
	tempBuilder->AddRef();
	tempBuilder->construct(pdbFileBuilder);
	*builder = tempBuilder;
	return S_OK;
}


STDMETHODIMP CLLVMPDBFileBuilder::getDbiBuilder(ILLVMDbiStreamBuilder** builder)
{
	CComObject<CLLVMDbiStreamBuilder>* tempBuilder;
	CComObject<CLLVMDbiStreamBuilder>::CreateInstance(&tempBuilder);
	tempBuilder->AddRef();
	tempBuilder->construct(bumpPtrAllocator, pdbFileBuilder);
	*builder = tempBuilder;
	return S_OK;
}


STDMETHODIMP CLLVMPDBFileBuilder::getTpiBuilder(ILLVMTpiStreamBuilder** builder)
{
	CComObject<CLLVMTpiStreamBuilder>* tempBuilder;
	CComObject<CLLVMTpiStreamBuilder>::CreateInstance(&tempBuilder);
	tempBuilder->AddRef();
	tempBuilder->constructTpiBuilder(bumpPtrAllocator, pdbFileBuilder);
	*builder = tempBuilder;
	return S_OK;
}


STDMETHODIMP CLLVMPDBFileBuilder::getIpiBuilder(ILLVMTpiStreamBuilder** builder)
{
	CComObject<CLLVMTpiStreamBuilder>* tempBuilder;
	CComObject<CLLVMTpiStreamBuilder>::CreateInstance(&tempBuilder);
	tempBuilder->AddRef();
	tempBuilder->constructIpiBuilder(bumpPtrAllocator, pdbFileBuilder);
	*builder = tempBuilder;
	return S_OK;
}


STDMETHODIMP CLLVMPDBFileBuilder::getGsiBuilder(ILLVMGSIStreamBuilder** builder)
{
	CComObject<CLLVMGSIStreamBuilder>* tempBuilder;
	CComObject<CLLVMGSIStreamBuilder>::CreateInstance(&tempBuilder);
	tempBuilder->AddRef();
	tempBuilder->construct(bumpPtrAllocator, pdbFileBuilder);
	*builder = static_cast<ILLVMGSIStreamBuilder*>(tempBuilder);
	return S_OK;
}


STDMETHODIMP CLLVMPDBFileBuilder::getStringTableBuilder(ILLVMPDBStringTableBuilder** builder)
{
	CComObject<CLLVMPDBStringTableBuilder>* tempBuilder;
	CComObject<CLLVMPDBStringTableBuilder>::CreateInstance(&tempBuilder);
	tempBuilder->AddRef();
	tempBuilder->construct(pdbFileBuilder);
	*builder = tempBuilder;
	return S_OK;
}


STDMETHODIMP CLLVMPDBFileBuilder::commit(BSTR Filename, GUID* Guid)
{
	return checkLLVMErrorAndSetCOMError(pdbFileBuilder->commit(utf_to_utf<char>(Filename), 
		reinterpret_cast<llvm::codeview::GUID*>(Guid)), GetObjectCLSID(), IID_ILLVMPDBFileBuilder);
}