#include "UiManager.h"
#include "Loggers/LoggerSingleton.h"



using namespace DreamEngine::Core::UI;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::Loggers;

UiManager UiManager::m_sUiManager;

void UiManager::Initialize(GLFWwindow* window, int width, int height)
{
    LoggerSingleton::Instance().LogTrace("UiManager::Initialize -> Start");
    m_sUiManager = UiManager();
    m_sUiManager.m_pUiHandler->Initialize(window, width, height);
}

UiInstance* UiManager::Create(const UiContent* content)
{
    return m_sUiManager.m_pUiHandler->Create(content);
}

void UiManager::Destroy(UiInstance* instance)
{
    m_sUiManager.m_pUiHandler->Destroy(instance);
}

void UiManager::Update()
{
    m_sUiManager.m_pUiHandler->Update();
}

void UiManager::RemoveContents()
{
    m_sUiManager.m_pUiHandler->RemoveContents();
}

void UiManager::BeginRender(const int width, const int height)
{
    m_sUiManager.m_pUiHandler->BeginRender(width, height);
}

void UiManager::Render()
{
    m_sUiManager.m_pUiHandler->Render();
}

void UiManager::EndRender()
{
    m_sUiManager.m_pUiHandler->EndRender();
}

void UiManager::Shutdown()
{
    return m_sUiManager.m_pUiHandler->Shutdown();
}

void UiManager::AddFont(Font* font)
{
    m_sUiManager.m_pUiHandler->AddFont(font);
}

void UiManager::Set(UiInstance* instance, const std::string& prop, std::string& value)
{
    m_sUiManager.m_pUiHandler->Set(instance, prop, value);
}

void UiManager::Set(UiInstance* instance, const std::string& prop, int value)
{
    m_sUiManager.m_pUiHandler->Set(instance, prop, value);
}

void UiManager::Set(UiInstance* instance, const std::string& prop, float value)
{
    m_sUiManager.m_pUiHandler->Set(instance, prop, value);
}

void UiManager::BindOnClickCallback(UiInstance* instance, const std::string& event, const std::function<void()> callback)
{
    m_sUiManager.m_pUiHandler->BindOnClickCallback(instance, event, callback);
}

bool UiManager::ProcessMouseMove(const int x, const int y)
{
    return m_sUiManager.m_pUiHandler->ProcessMouseMove(x, y);
}

bool UiManager::ProcessMouseButton(const int button, const int action, const int mods)
{
    return m_sUiManager.m_pUiHandler->ProcessMouseButton(button, action, mods);
}

bool UiManager::ProcessKey(const int key, const int scancode, const int action, const int mods)
{
    return m_sUiManager.m_pUiHandler->ProcessKey(key, scancode, action, mods);
}

bool UiManager::ProcessScroll(const double xOffset, const double yOffset)
{
    return m_sUiManager.m_pUiHandler->ProcessScroll(xOffset, yOffset);
}