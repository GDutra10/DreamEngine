#ifndef CORE_UI_I_UI_HANDLER_H_
#define CORE_UI_I_UI_HANDLER_H_

#include "UiContent.h"
#include "GLFW/glfw3.h"
#include "GameSystem/Game.h"

namespace DreamEngine::Core::UI
{
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::GameSystem;

class IUiHandler
{
   public:
    virtual void Initialize(GLFWwindow* window, int width, int height) = 0;
    virtual void* Create(const UiContent* content) = 0;
    virtual void Update() = 0;
    virtual void RemoveContents() = 0;
    virtual void BeginRender(Game* game) = 0;
    virtual void Render(Game* game) = 0;
    virtual void EndRender() = 0;
    virtual void Shutdown() = 0;

    // Input event processing - returns true if UI consumed the event
    virtual bool ProcessMouseMove(int x, int y) = 0;
    virtual bool ProcessMouseButton(int button, int action, int mods) = 0;
    virtual bool ProcessKey(int key, int scancode, int action, int mods) = 0;
    virtual bool ProcessScroll(double xoffset, double yoffset) = 0;
};
}  // namespace DreamEngine::Core::UI
#endif