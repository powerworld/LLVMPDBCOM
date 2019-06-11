// LLVMPDBStringTableBuilder.cpp : Implementation of CLLVMPDBStringTableBuilder

#include "pch.h"
#include "LLVMPDBStringTableBuilder.h"
#include "LLVMDebugStringTableSubsection.h"

using namespace boost::locale::conv;

// CLLVMPDBStringTableBuilder

void CLLVMPDBStringTableBuilder::construct(const std::shared_ptr<pdb::PDBFileBuilder>& thePDBFileBuilder)
{
	pdbFileBuilder = thePDBFileBuilder;
	pdbStringTableBuilder = pdbFileBuilder->getStringTableBuilder();
}

STDMETHODIMP CLLVMPDBStringTableBuilder::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ILLVMPDBStringTableBuilder
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMPDBStringTableBuilder::insert(BSTR S, ULONG* Res)
{
	*Res = pdbStringTableBuilder->insert(utf_to_utf<char>(S));
	return S_OK;
}


STDMETHODIMP CLLVMPDBStringTableBuilder::getIdForString(BSTR S, ULONG* Res)
{
	*Res = pdbStringTableBuilder->getIdForString(utf_to_utf<char>(S));
	return S_OK;
}


STDMETHODIMP CLLVMPDBStringTableBuilder::getStringForId(ULONG Id, BSTR* Res)
{
	CComBSTR temp(
		utf_to_utf<wchar_t>(pdbStringTableBuilder->getStringForId(Id).str()).c_str());
	return temp.CopyTo(Res);
}


STDMETHODIMP CLLVMPDBStringTableBuilder::setStrings(ILLVMDebugStringTableSubsection* Strings)
{
	CComPtr<ILLVMDebugStringTableSubsection> pStrings(Strings);
	pdbStringTableBuilder->setStrings(
		*static_cast<CLLVMDebugStringTableSubsection*>(pStrings.p)->getShared());
	return S_OK;
}
