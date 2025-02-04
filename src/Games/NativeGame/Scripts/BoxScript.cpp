#include "BoxScript.h"

using namespace DreamEngine::Core::ECS::Components;

void BoxScript::Start() 
{
    m_transformComponent = &entity->GetComponent<TransformComponent>();
}

void BoxScript::Update()
{
    m_transformComponent->transform = glm::rotate(m_transformComponent->transform, 0.01f, glm::vec3(0.0f, 1.0f, 1.0f));
}
