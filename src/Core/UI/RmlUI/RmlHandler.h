#ifndef CORE_UI_RML_RML_HANDLER_H_
#define CORE_UI_RML_RML_HANDLER_H_

#include "CustomRenderInterface_GL3.h"
#include "../UiContent.h"
#include "../IUiHandler.h"
#include "RmlUi/Core/Context.h"
#include "RmlUi_Platform_GLFW.h"
#include "RmlUi_Renderer_GL3.h"
#include "GLFW/glfw3.h"
#include "GameSystem/Game.h"

namespace DreamEngine::Core::UI::RmlUI
{
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::GameSystem;

class RmlHandler final : public IUiHandler
{
   public:
    void Initialize(GLFWwindow* window, int width, int height) override;
    void* Create(const UiContent* content) override;
    void Destroy(void* instance) override;
    void Update() override;
    void RemoveContents() override;
    void BeginRender(Game* game) override;
    void Render(Game* game) override;
    void EndRender() override;
    void Shutdown() override;

    // Input event processing - returns true if UI consumed the event
    bool ProcessMouseMove(int x, int y) override;
    bool ProcessMouseButton(int button, int action, int mods) override;
    bool ProcessKey(int key, int scancode, int action, int mods) override;
    bool ProcessScroll(double xOffset, double yOffset) override;

   private:
    static Rml::Context* m_spContext;
    static SystemInterface_GLFW* m_spSystemInterface;
    static CustomRenderInterface_GL3* m_spRenderInterface;
    static void ShutdownInterfaces();
    Rml::Input::KeyIdentifier ConvertKey(int glfwKey);
    static int ConvertMouseButton(int glfwButton);
    static int GetKeyModifiers(GLFWwindow* window);
};
}  // namespace DreamEngine::Core::UI
#endif