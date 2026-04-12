#include "Script.h"
#include "ScriptEngine.h"

using namespace DreamEngine::Core::Scripting;

Script::Script(const std::string& className, const std::string& assemblyName)
{
    m_className = className;
    m_assemblyName = assemblyName;
}

std::string Script::GetAssemblyName() const
{
    return m_assemblyName;
}

std::string Script::GetClassName() const
{
    return m_className;
}
