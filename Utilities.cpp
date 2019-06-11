#include "pch.h"
#include "Utilities.h"

using namespace ATL;

HRESULT checkLLVMErrorAndSetCOMError(Error e, const CLSID& clsid, const IID& iid)
{
	std::string errors;
	handleAllErrors(
		std::move(e),
		[&](const ErrorInfoBase& ei) {
			if (!errors.empty())
				errors.append("\n");
			errors.append(ei.message());
		});
	if (!errors.empty())
		return AtlReportError(clsid, errors.c_str(), iid);
	return S_OK;
}