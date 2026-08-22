#include "BoxScript.h"

#include "Inputs/Input.h"
#include "Resources/ResourceManager.h"
#include "Systems/AudioSystem.h"

using namespace DreamEngine::Core::Inputs;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Systems;

void BoxScript::Start() 
{
    m_transformComponent = &entity->GetComponent<TransformComponent>();
}

void BoxScript::Update()
{
    m_transformComponent->transform = glm::rotate(m_transformComponent->transform, 0.01f, glm::vec3(0.0f, 1.0f, 1.0f));

    if (Input::IsKeyPressed(Key::F8))
    {
        AudioPlayOptions options;
        options.spatial = true;
        options.loop = false;
        options.bus = AudioBus::SFX;

        //AudioSystem::Instance().Play("explosion", options);
        AudioSystem::Instance().Play(this->entity->GetId(), "explosion", options);
    }
}
