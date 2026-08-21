#include "CameraScript.h"

#include "Inputs/Input.h"

using namespace DreamEngine::Core::Inputs;

void CameraScript::Start()
{
    this->m_pTransformComponent = &this->entity->GetComponent<TransformComponent>();
}

void CameraScript::Update()
{
    if (Input::IsKeyDown(Key::W))
    {
        glm::vec3 position = this->m_pTransformComponent->GetPosition();
        position.z -= m_speed;
        this->m_pTransformComponent->SetPosition(position);
    }

    if (Input::IsKeyDown(Key::S))
    {
        glm::vec3 position = this->m_pTransformComponent->GetPosition();
        position.z += m_speed;
        this->m_pTransformComponent->SetPosition(position);
    }

    if (Input::IsKeyDown(Key::A))
    {
        glm::vec3 position = this->m_pTransformComponent->GetPosition();
        position.x -= m_speed;
        this->m_pTransformComponent->SetPosition(position);
    }

    if (Input::IsKeyDown(Key::D))
    {
        glm::vec3 position = this->m_pTransformComponent->GetPosition();
        position.x += m_speed;
        this->m_pTransformComponent->SetPosition(position);
    }
}

