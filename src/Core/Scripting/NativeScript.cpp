#include "NativeScript.h"
#include <ECS/Components/NativeScriptComponent.h>

using namespace DreamEngine::Core::Scripting;

void NativeScript::Initialize() 
{
    if (m_wasInitialized)
        return;

    Start();

    m_wasInitialized = true;
}
