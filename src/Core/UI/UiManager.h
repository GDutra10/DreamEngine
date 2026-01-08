#ifndef CORE_UI_UI_MANAGER_H_
#define CORE_UI_UI_MANAGER_H_

#include "IUiHandler.h"
#include "UiContent.h"
#include "GLFW/glfw3.h"
#include "GameSystem/Game.h"
#include "RmlUI/RmlHandler.h"

namespace DreamEngine::Core::UI
{
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::UI::RmlUI;

class CORE_API UiManager
{
   public:
    static void Initialize(GLFWwindow* window, int width, int height);
    static UiInstance* Create(const UiContent* content);
    static void Destroy(UiInstance* instance);
    static void Update();
    static void RemoveContents();
    static void BeginRender(Game* game);
    static void Render(Game* game);
    static void EndRender();
    static void Shutdown();

    // hook data
    static void Set(UiInstance* instance, const std::string& prop, std::string& value);
    static void Set(UiInstance* instance, const std::string& prop, int value);
    static void Set(UiInstance* instance, const std::string& prop, float value);
    static void BindOnClickCallback(UiInstance* instance, const std::string& prop, const std::function<void()> callback);

    // Input event processing - returns true if UI consumed the event
    static bool ProcessMouseMove(int x, int y);
    static bool ProcessMouseButton(int button, int action, int mods);
    static bool ProcessKey(int key, int scancode, int action, int mods);
    static bool ProcessScroll(double xOffset, double yOffset);
private:
    UiManager() : m_pUiHandler(new RmlHandler()) {}
    static UiManager m_sUiManager;
    IUiHandler* m_pUiHandler;
};
}  // namespace DreamEngine::Core::UI
#endif