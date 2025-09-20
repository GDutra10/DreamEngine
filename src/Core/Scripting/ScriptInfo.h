#ifndef CORE_SCRIPTING_SCRIPT_INFO_H_
#define CORE_SCRIPTING_SCRIPT_INFO_H_

#include "CoreExport.h"

namespace DreamEngine::Core::Scripting
{
struct CORE_API ScriptInfo
{
    char* AssemblyName;
    char* Name;
};
}  // namespace DreamEngine::Core::Scripts
#endif