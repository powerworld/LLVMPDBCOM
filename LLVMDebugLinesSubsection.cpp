// LLVMDebugLinesSubsection.cpp : Implementation of CLLVMDebugLinesSubsection

#include "pch.h"
#include "LLVMDebugLinesSubsection.h"

using namespace boost::locale::conv;

// CLLVMDebugLinesSubsection

STDMETHODIMP CLLVMDebugLinesSubsection::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ILLVMDebugLinesSubsection
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CLLVMDebugLinesSubsection::construct(ILLVMDebugChecksumsSubsection* Checksums, ILLVMDebugStringTableSubsection* Strings)
{
	// manage interface lifetimes for duration of function
	CComPtr<ILLVMDebugChecksumsSubsection> pChecksums(Checksums);
	CComPtr<ILLVMDebugStringTableSubsection> pStrings(Strings);
	debugChecksumsSubsection = static_cast<CLLVMDebugChecksumsSubsection*>(pChecksums.p)->getShared();
	debugStringTableSubsection = static_cast<CLLVMDebugStringTableSubsection*>(pStrings.p)->getShared();
	debugLinesSubsection = std::make_shared<codeview::DebugLinesSubsection>(
		*debugChecksumsSubsection, *debugStringTableSubsection);
	debugSubsection = debugLinesSubsection;
	return S_OK;
}


STDMETHODIMP CLLVMDebugLinesSubsection::setRelocationAddress(USHORT Segment, ULONG Offset)
{
	debugLinesSubsection->setRelocationAddress(Segment, Offset);
	return S_OK;
}


STDMETHODIMP CLLVMDebugLinesSubsection::setCodeSize(ULONG Size)
{
	debugLinesSubsection->setCodeSize(Size);
	return S_OK;
}


STDMETHODIMP CLLVMDebugLinesSubsection::createBlock(BSTR FileName)
{
	debugLinesSubsection->createBlock(utf_to_utf<char>(FileName));
	return S_OK;
}


STDMETHODIMP CLLVMDebugLinesSubsection::addLineInfo(ULONG Offset, ULONG StartLine, ULONG EndLine, VARIANT_BOOL IsStatement)
{
	codeview::LineInfo li(StartLine, EndLine, IsStatement != VARIANT_FALSE);
	debugLinesSubsection->addLineInfo(Offset, li);
	return S_OK;
}
