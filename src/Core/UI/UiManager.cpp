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

void* UiManager::Create(const UiContent* content)
{
    return m_sUiManager.m_pUiHandler->Create(content);
}

void UiManager::Update()
{
    m_sUiManager.m_pUiHandler->Update();
}

void UiManager::RemoveContents()
{
    m_sUiManager.m_pUiHandler->RemoveContents();
}

void UiManager::BeginRender(Game* game)
{
    m_sUiManager.m_pUiHandler->BeginRender(game);
}

void UiManager::Render(Game* game)
{
    m_sUiManager.m_pUiHandler->Render(game);
}

void UiManager::EndRender()
{
    m_sUiManager.m_pUiHandler->EndRender();
}

void UiManager::Shutdown()
{
    return m_sUiManager.m_pUiHandler->Shutdown();
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