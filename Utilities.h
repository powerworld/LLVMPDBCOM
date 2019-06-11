#pragma once

using namespace llvm;

HRESULT checkLLVMErrorAndSetCOMError(Error e, const CLSID& clsid, const IID& iid);