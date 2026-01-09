#include "HudScript.h"

#include "Inputs/Input.h"
#include "Loggers/LoggerSingleton.h"
#include "UI/UiManager.h"

using namespace DreamEngine::Core::Inputs;


void HudScript::Start()
{
    m_pUiComponent = &this->entity->GetComponent<UiComponent>();
    UiManager::Set(m_pUiComponent->instance, "health", m_health);
    UiManager::Set(m_pUiComponent->instance, "ammo", m_ammo);
    UiManager::Set(m_pUiComponent->instance, "name", m_name);

    UiManager::BindOnClickCallback(m_pUiComponent->instance, "btnReset", [this](){
        DreamEngine::Core::Loggers::LoggerSingleton::Instance().LogInfo("Reset button clicked");
        
        this->m_health = 0;
        this->m_ammo = 0;
        this->m_degree = 0.0;
        this->m_name = "John";

        UiManager::Set(m_pUiComponent->instance, "health", m_health);
        UiManager::Set(m_pUiComponent->instance, "ammo", m_ammo);
        UiManager::Set(m_pUiComponent->instance, "name", m_name);
        UiManager::Set(m_pUiComponent->instance, "degree", m_degree);
    });
}

void HudScript::Update()
{
    if (Input::IsPressed(Key::F1))
    {
        m_health++;
        UiManager::Set(m_pUiComponent->instance, "health", m_health);
    }

    if (Input::IsPressed(Key::F2))
    {
        m_ammo++;
        UiManager::Set(m_pUiComponent->instance, "ammo", m_ammo);
    }

    if (Input::IsPressed(Key::F3))
    {
        m_name += "!";
        UiManager::Set(m_pUiComponent->instance, "name", m_name);
    }

    if (Input::IsPressed(Key::F4))
    {
        m_degree += 0.01f;
        UiManager::Set(m_pUiComponent->instance, "degree", m_degree);
    }
}
