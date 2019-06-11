// LLVMDebugChecksumsSubsection.cpp : Implementation of CLLVMDebugChecksumsSubsection

#include "pch.h"
#include "LLVMBumpPtrAllocator.h"
#include "LLVMDebugChecksumsSubsection.h"
#include "LLVMDebugStringTableSubsection.h"

using namespace boost::locale::conv;

// CLLVMDebugChecksumsSubsection

STDMETHODIMP CLLVMDebugChecksumsSubsection::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ILLVMDebugChecksumsSubsection
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMDebugChecksumsSubsection::construct(ILLVMBumpPtrAllocator* Allocator, 
	ILLVMDebugStringTableSubsection* Strings)
{
	CComPtr<ILLVMBumpPtrAllocator> pAllocator(Allocator);
	CComPtr<ILLVMDebugStringTableSubsection> pStrings(Strings);
	bumpPtrAllocator = static_cast<CLLVMBumpPtrAllocator*>(pAllocator.p)->getShared();
	debugStringTableSubsection = static_cast<CLLVMDebugStringTableSubsection*>(pStrings.p)->getShared();
	debugChecksumsSubsection = std::make_shared<codeview::DebugChecksumsSubsection>(*debugStringTableSubsection);
	debugSubsection = debugChecksumsSubsection;
	return S_OK;
}


STDMETHODIMP CLLVMDebugChecksumsSubsection::addChecksum(BSTR FileName, LLVM_FileChecksumKind Kind, VARIANT Bytes)
{
	if (Bytes.vt == VT_NULL)
		debugChecksumsSubsection->addChecksum(utf_to_utf<char>(FileName),
			static_cast<codeview::FileChecksumKind>(Kind), llvm::None);
	else if (Bytes.vt == (VT_ARRAY | VT_UI1)) {
		HRESULT hres = SafeArrayLock(Bytes.parray);
		if (FAILED(hres)) return hres;
		BOOST_SCOPE_EXIT(&Bytes) {
			SafeArrayUnlock(Bytes.parray);
		} BOOST_SCOPE_EXIT_END
		if (SafeArrayGetDim(Bytes.parray) != 1) return E_INVALIDARG;

		LONG l, u;
		hres = SafeArrayGetLBound(Bytes.parray, 1, &l);
		if (FAILED(hres)) return hres;
		hres = SafeArrayGetUBound(Bytes.parray, 1, &u);
		if (FAILED(hres)) return hres;

		uint32_t len = u - l + 1;
		uint8_t* storage = bumpPtrAllocator->Allocate<uint8_t>(len);
		std::memcpy(storage, Bytes.parray->pvData, len);
		debugChecksumsSubsection->addChecksum(utf_to_utf<char>(FileName),
			static_cast<codeview::FileChecksumKind>(Kind),
			ArrayRef<uint8_t>(storage, len));
	}
	else
		return E_INVALIDARG;
	return S_OK;
}