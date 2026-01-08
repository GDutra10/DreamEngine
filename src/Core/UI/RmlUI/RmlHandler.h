#ifndef CORE_UI_RML_RML_HANDLER_H_
#define CORE_UI_RML_RML_HANDLER_H_

#include "CustomRenderInterface_GL3.h"
#include "RmlUiInstance.h"
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
    UiInstance* Create(const UiContent* content) override;
    void Destroy(UiInstance* instance) override;
    void Update() override;
    void RemoveContents() override;
    void BeginRender(Game* game) override;
    void Render(Game* game) override;
    void EndRender() override;
    void Shutdown() override;

    // hook data
    void Set(UiInstance* instance, std::string prop, std::string& value) override;
    void Set(UiInstance* instance, std::string prop, int value) override;
    void Set(UiInstance* instance, std::string prop, float value) override;
    void BindOnClickCallback(UiInstance* instance, const std::string& prop, const std::function<void()> callback) override;

    // Input event processing - returns true if UI consumed the event
    bool ProcessMouseMove(int x, int y) override;
    bool ProcessMouseButton(int button, int action, int mods) override;
    bool ProcessKey(int key, int scancode, int action, int mods) override;
    bool ProcessScroll(double xOffset, double yOffset) override;

   private:
    static Rml::Context* m_spContext;
    static SystemInterface_GLFW* m_spSystemInterface;
    static CustomRenderInterface_GL3* m_spRenderInterface;
    static std::vector<RmlUiInstance*> m_sUiInstances;
    static void ShutdownInterfaces();
    Rml::Input::KeyIdentifier ConvertKey(int glfwKey);
    static int ConvertMouseButton(int glfwButton);
    static int GetKeyModifiers(GLFWwindow* window);
    static void Set(UiInstance*& instance, const std::string& prop, const Rml::Variant& value);
    static void Reload(RmlUiInstance* ui);
    static void AttachEvents(const RmlUiInstance* ui);
    static bool TryAddOnClickEventListener(const RmlUiInstance* ui, const std::string& id, const std::function<void()> callback);
};
}  // namespace DreamEngine::Core::UI
#endif