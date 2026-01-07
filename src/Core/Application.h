#ifndef CORE_APPLICATION_H_
#define CORE_APPLICATION_H_

#include <string>

//#include <GLFW/glfw3.h>
#include "Inputs/Key.h"
#include "Inputs/KeyState.h"
#include "Inputs/MouseButton.h"
#include "Render/RenderPipeline.h"
#include "Render/RenderType.h"
#include "Render/OpenGL/OpenGLRenderAPI.h"
#include "Scripting/ScriptEngine.h"
#include "Sync/InputData.h"
#include "Sync/GameData.h"
#include "UI/UiManager.h"

namespace DreamEngine::Core
{

using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Core::Inputs;
using namespace DreamEngine::Core::Scripting;
using namespace DreamEngine::Core::UI;

class CORE_API Application
{
   public:
    static Application& Instance();
    void Run(const int width, const int height, const std::string& name, RenderType renderType);
    void Run(const int width, const int height, const std::string& name, RenderType renderType, Game* game);
    [[nodiscard]] GLFWwindow* GetWindow() const;
    [[nodiscard]] RenderAPI* GetRenderAPI() const { return m_renderAPI; }
    [[nodiscard]] RenderType GetRenderType() const { return m_renderType; }
    [[nodiscard]] Game* GetGame() const { return m_game; }
    [[nodiscard]] ScriptEngine* GetScriptEngine() const { return m_scriptEngine; }
    [[nodiscard]] GameData* GetGameData() { return m_gameData; }
    float GetDeltaTime() const { return m_deltaTime; }
    bool GetIsFocused() const;
    void Close() const;
  private:
    // the completion time in seconds since the last frame
    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;
    Game* m_game = nullptr;
    GLFWwindow* m_window = nullptr;
    RenderAPI* m_renderAPI = nullptr;
    RenderPipeline* m_renderPipeline = nullptr;
    RenderType m_renderType = none;
    ScriptEngine* m_scriptEngine = nullptr;
    GameData* m_gameData = new GameData();
    void SetRenderAPI(const RenderType renderType);
    static void GLFWInit();
    void InitializeWindow(const int width, const int height, const std::string& name);

    static bool m_sFirstMouseCallback;
    static float m_sMouseLastX;
    static float m_sMouseLastY;
    static void MousePositionCallback(GLFWwindow* window, double xPosIn, double yPosIn);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static MouseButton GetMouseButtonByGLFW(int mouseButton);
    static Key GetKeyByGLFWKey(int key);
    static KeyState GetMouseKeyEventByGLFW(int action);
};
}  // namespace DreamEngine::Core
#endif