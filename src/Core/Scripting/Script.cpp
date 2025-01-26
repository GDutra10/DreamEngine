#include "Script.h"
#include "ScriptEngine.h"

using namespace DreamEngine::Core::Scripting;

Script::Script(const std::string& className, const std::string& assemblyName)
{
    m_className = className;
    m_assemblyName = assemblyName;
}

void* Script::Update(void* instance, EntityData* entityData)
{
    if (instance == nullptr)
        instance = ScriptEngine::CreateInstance(m_assemblyName.c_str(), m_className.c_str());

    ScriptEngine::Update(instance, entityData);

    return instance;
}

void* Script::Destroy(void* instance)
{
    if (instance != nullptr)
        ScriptEngine::ReleaseInstance(instance);
    
    return nullptr;
}

std::string Script::GetAssemblyName() const
{
    return m_assemblyName;
}

std::string Script::GetClassName() const
{
    return m_className;
}
