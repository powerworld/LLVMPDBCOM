// LLVMPDBUtilities.cpp : Implementation of CLLVMPDBUtilities

#include "pch.h"
#include "LLVMPDBUtilities.h"
#include "Utilities.h"

// CLLVMPDBUtilities

STDMETHODIMP CLLVMPDBUtilities::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ILLVMPDBUtilities
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMPDBUtilities::hashTypeRecord(VARIANT Type, ULONG* Res)
{
	if (Type.vt != (VT_ARRAY | VT_UI1)) return E_INVALIDARG;
	HRESULT hres = SafeArrayLock(Type.parray);
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

	codeview::RecordPrefix* prefix = static_cast<codeview::RecordPrefix*>(Type.parray->pvData);
	{
		// need expected to go out of scope before the unlock label to avoid errors
		auto expected = pdb::hashTypeRecord(
			codeview::CVType(prefix, u - l + 1));
			hres = checkLLVMErrorAndSetCOMError(expected.takeError(), GetObjectCLSID(),
			IID_ILLVMPDBUtilities);
		if (SUCCEEDED(hres)) *Res = *expected;
	}

	return hres;
}
