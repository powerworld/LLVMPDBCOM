// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// screws up LLVM headers
#define NOMINMAX

// add headers that you want to pre-compile here
#include <cstring>
#include <vector>
#include <list>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/DebugInfo/CodeView/CodeView.h>
#include <llvm/DebugInfo/CodeView/DebugChecksumsSubsection.h>
#include <llvm/DebugInfo/CodeView/DebugLinesSubsection.h>
#include <llvm/DebugInfo/CodeView/DebugStringTableSubsection.h>
#include <llvm/DebugInfo/CodeView/DebugSubsection.h>
#include <llvm/DebugInfo/CodeView/Line.h>
#include <llvm/DebugInfo/CodeView/RecordSerialization.h>
#include <llvm/DebugInfo/CodeView/SymbolDeserializer.h>
#include <llvm/DebugInfo/MSF/MSFBuilder.h>
#include <llvm/DebugInfo/PDB/Native/DbiStreamBuilder.h>
#include <llvm/DebugInfo/PDB/Native/DbiModuleDescriptorBuilder.h>
#include <llvm/DebugInfo/PDB/Native/InfoStreamBuilder.h>
#include <llvm/DebugInfo/PDB/Native/GSIStreamBuilder.h>
#include <llvm/DebugInfo/PDB/Native/PDBFileBuilder.h>
#include <llvm/DebugInfo/PDB/Native/PDBStringTableBuilder.h>
#include <llvm/DebugInfo/PDB/Native/RawConstants.h>
#include <llvm/DebugInfo/PDB/Native/RawTypes.h>
#include <llvm/DebugInfo/PDB/Native/TpiStreamBuilder.h>
#include <llvm/DebugInfo/PDB/Native/TpiHashing.h>
#include <llvm/Object/COFF.h>
#include <llvm/Support/BinaryStreamReader.h>
#include <llvm/Support/Error.h>

#include <boost/locale.hpp>
#include <boost/optional.hpp>
#include <boost/scope_exit.hpp>

#include "framework.h"

#endif //PCH_H
