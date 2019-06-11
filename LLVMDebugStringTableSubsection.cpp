// LLVMDebugStringTableSubsection.cpp : Implementation of CLLVMDebugStringTableSubsection

#include "pch.h"
#include "LLVMDebugStringTableSubsection.h"

using namespace boost::locale::conv;

// CLLVMDebugStringTableSubsection

STDMETHODIMP CLLVMDebugStringTableSubsection::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ILLVMDebugStringTableSubsection
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMDebugStringTableSubsection::insert(BSTR S, ULONG* Res)
{
	if (S == nullptr)
		*Res = debugStringTableSubsection->insert("");
	else
		*Res = debugStringTableSubsection->insert(utf_to_utf<char>(S));
	return S_OK;
}


STDMETHODIMP CLLVMDebugStringTableSubsection::getIdForString(BSTR S, ULONG* Res)
{
	*Res = debugStringTableSubsection->getIdForString(utf_to_utf<char>(S));
	return S_OK;
}


STDMETHODIMP CLLVMDebugStringTableSubsection::getStringForId(ULONG Id, BSTR* Res)
{
	CComBSTR temp(
		utf_to_utf<wchar_t>(debugStringTableSubsection->getStringForId(Id).str()).c_str());
	return temp.CopyTo(Res);
}
