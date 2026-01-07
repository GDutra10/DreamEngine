#include "Application.h"

#include "Render/OpenGL/OpenGLRenderAPI.h"
#include "../Core/Inputs/Input.h"
#include "../Core/Resources/ResourceManager.h"
#include "../Core/Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Inputs;
using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Render::OpenGL;

bool Application::m_sFirstMouseCallback = true;
float Application::m_sMouseLastX = 0.0f;
float Application::m_sMouseLastY = 0.0f;

Application& Application::Instance()
{
    static Application application;

    return application;
}

void Application::Run(int width, int height, const std::string& name, const RenderType renderType, Game* game)
{

    try
    {
        SetRenderAPI(renderType);

        if (m_renderAPI == nullptr)
            throw std::exception("Render is null!");

        GLFWInit();
        InitializeWindow(width, height, name);

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::exception("Failed to initialized GLAD");

        if (game->hasScriptEngine)
            m_scriptEngine = new ScriptEngine();

        m_game = game;

        m_renderAPI->Initialize(width, height);
        UiManager::Initialize(m_window, width, height);

        // TODO: Load the assets(ResourceManager) by the first scene file
        // TODO: Initialize entities from the first scene file

        while (true)
        {
            if (m_game->GetActiveScene() == nullptr)
                m_game->ChangeActiveScene();

            if (glfwWindowShouldClose(m_window))
                break;

             // update window size
            glfwGetWindowSize(m_window, &width, &height);

            m_game->width = width;
            m_game->height = height;

            // show cursor
            glfwSetInputMode(m_window, GLFW_CURSOR, m_game->GetActiveScene()->GetShowCursor() ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

            // per-frame time logic
            const float currentFrame = static_cast<float>(glfwGetTime());
            m_deltaTime = currentFrame - m_lastFrame;
            m_lastFrame = currentFrame;

            // update scene and entities
            m_game->GetActiveScene()->Update(m_deltaTime);

            // render
            if (const int isMinimized = glfwGetWindowAttrib(m_window, GLFW_ICONIFIED); !isMinimized)
            {
                // render default
                m_renderAPI->BeforeRender();
                m_renderAPI->Render(m_game);

                UiManager::BeginRender(m_game);
                UiManager::Render(m_game);
                UiManager::EndRender();

                // render in each fbo
                std::vector<FrameBuffer*> frameBuffers = m_renderAPI->GetFrameBuffers();

                for (FrameBuffer* frameBuffer : frameBuffers)
                {
                    if (frameBuffer == nullptr)
                        continue;

                    frameBuffer->Attach();
                    
                    m_renderAPI->Render(m_game);
                    
                    UiManager::BeginRender(m_game);
                    UiManager::Render(m_game);
                    UiManager::EndRender();
                     
                    frameBuffer->Detach();
                }

                int displayW, displayH;
                
                glfwGetFramebufferSize(m_window, &displayW, &displayH);
                m_renderAPI->AfterRender(m_game->width, m_game->height);
            }

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }

        UiManager::Shutdown();
        glfwTerminate();
    }
    catch (const std::exception& e)
    {
        std::string exception = e.what();
        LoggerSingleton::Instance().LogError("Exception: " + exception);
        glfwTerminate();
    }
}

GLFWwindow* Application::GetWindow() const
{
    return m_window;
}

void Application::Close() const
{
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void Application::Run(const int width, const int height, const std::string& name, RenderType renderType)
{
    // TODO: get from embeded dll
    Game* game = nullptr;

    Run(width, height, name, renderType, game);
}

void Application::SetRenderAPI(const RenderType renderType)
{
    m_renderType = renderType;

    switch (renderType)
    {
        case RenderType::openGL:
            m_renderAPI = new OpenGLRenderAPI();
            break;
        case RenderType::vulkan:
        case RenderType::none:
        default: 
            break;
    }
}

void Application::GLFWInit()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  // Debug context

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void Application::InitializeWindow(const int width, const int height, const std::string& name)
{
    m_window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);

    if (m_window == nullptr)
        throw std::exception("Window is null");

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, FrameBufferSizeCallback);
    /*glfwSetScrollCallback(m_window, ScrollCallback);*/
    glfwSetCursorPosCallback(m_window, MousePositionCallback);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
    glfwSetKeyCallback(m_window, KeyCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwPollEvents();
}

bool Application::GetIsFocused() const
{
    return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
}

void Application::MousePositionCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{
    const auto xPos = static_cast<float>(xPosIn);
    const auto yPos = static_cast<float>(yPosIn);

    if (m_sFirstMouseCallback)
    {
        m_sMouseLastX = xPos;
        m_sMouseLastY = yPos;
        m_sFirstMouseCallback = false;
    }

    const float xOffset = xPos - m_sMouseLastX;
    const float yOffset = m_sMouseLastY - yPos;  // reversed since y-coordinates go from bottom to top

    m_sMouseLastX = xPos;
    m_sMouseLastY = yPos;

    UiManager::ProcessMouseMove(static_cast<int>(xPos), static_cast<int>(yPos));
    Input::SetMousePosition({xOffset, yOffset});
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    UiManager::ProcessMouseButton(button, action, mods);
    Input::SetMouseState(GetMouseButtonByGLFW(button), GetMouseKeyEventByGLFW(action));
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    UiManager::ProcessKey(key, scancode, action, mods);
    Input::SetKeyState(GetKeyByGLFWKey(key), GetMouseKeyEventByGLFW(action));
}

void Application::FrameBufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    Application::Instance().GetRenderAPI()->RescaleFrameBuffers(width, height);
}

MouseButton Application::GetMouseButtonByGLFW(const int mouseButton)
{
    switch (mouseButton)
    {
        case GLFW_MOUSE_BUTTON_1:
            return MOUSE_BUTTON_LEFT;
        case GLFW_MOUSE_BUTTON_2:
            return MOUSE_BUTTON_RIGHT;
        case GLFW_MOUSE_BUTTON_3:
            return MOUSE_BUTTON_MIDDLE;
        case GLFW_MOUSE_BUTTON_4:
            return MOUSE_BUTTON_4;
        case GLFW_MOUSE_BUTTON_5:
            return MOUSE_BUTTON_5;
        case GLFW_MOUSE_BUTTON_6:
            return MOUSE_BUTTON_6;
        case GLFW_MOUSE_BUTTON_7:
            return MOUSE_BUTTON_7;
        case GLFW_MOUSE_BUTTON_8:
            return MOUSE_BUTTON_8;
        default:
            throw std::exception("Mouse not mapped");
    }
}

Key Application::GetKeyByGLFWKey(const int key)
{
    switch (key)
    {
        case GLFW_KEY_KP_0:
            return KP_NUM_0;
        case GLFW_KEY_KP_1:
            return KP_NUM_1;
        case GLFW_KEY_KP_2:
            return KP_NUM_2;
        case GLFW_KEY_KP_3:
            return KP_NUM_3;
        case GLFW_KEY_KP_4:
            return KP_NUM_4;
        case GLFW_KEY_KP_5:
            return KP_NUM_5;
        case GLFW_KEY_KP_6:
            return KP_NUM_6;
        case GLFW_KEY_KP_7:
            return KP_NUM_7;
        case GLFW_KEY_KP_8:
            return KP_NUM_8;
        case GLFW_KEY_KP_9:
            return KP_NUM_9;
        case GLFW_KEY_0:
            return NUM_0;
        case GLFW_KEY_1:
            return NUM_1;
        case GLFW_KEY_2:
            return NUM_2;
        case GLFW_KEY_3:
            return NUM_3;
        case GLFW_KEY_4:
            return NUM_4;
        case GLFW_KEY_5:
            return NUM_5;
        case GLFW_KEY_6:
            return NUM_6;
        case GLFW_KEY_7:
            return NUM_7;
        case GLFW_KEY_8:
            return NUM_8;
        case GLFW_KEY_9:
            return NUM_9;
        case GLFW_KEY_A:
            return A;
        case GLFW_KEY_B:
            return B;
        case GLFW_KEY_C:
            return C;
        case GLFW_KEY_D:
            return D;
        case GLFW_KEY_E:
            return E;
        case GLFW_KEY_F:
            return F;
        case GLFW_KEY_G:
            return G;
        case GLFW_KEY_H:
            return H;
        case GLFW_KEY_I:
            return I;
        case GLFW_KEY_J:
            return J;
        case GLFW_KEY_K:
            return K;
        case GLFW_KEY_L:
            return L;
        case GLFW_KEY_M:
            return M;
        case GLFW_KEY_N:
            return N;
        case GLFW_KEY_O:
            return O;
        case GLFW_KEY_P:
            return P;
        case GLFW_KEY_Q:
            return Q;
        case GLFW_KEY_R:
            return R;
        case GLFW_KEY_S:
            return S;
        case GLFW_KEY_T:
            return T;
        case GLFW_KEY_U:
            return U;
        case GLFW_KEY_V:
            return V;
        case GLFW_KEY_W:
            return W;
        case GLFW_KEY_X:
            return X;
        case GLFW_KEY_Y:
            return Y;
        case GLFW_KEY_Z:
            return Z;
        case GLFW_KEY_F1:
            return F1;
        case GLFW_KEY_F2:
            return F2;
        case GLFW_KEY_F3:
            return F3;
        case GLFW_KEY_F4:
            return F4;
        case GLFW_KEY_F5:
            return F5;
        case GLFW_KEY_F6:
            return F6;
        case GLFW_KEY_F7:
            return F7;
        case GLFW_KEY_F8:
            return F8;
        case GLFW_KEY_F9:
            return F9;
        case GLFW_KEY_F10:
            return F10;
        case GLFW_KEY_F11:
            return F11;
        case GLFW_KEY_F12:
            return F12;
        case GLFW_KEY_LEFT_ALT:
            return LEFT_ALT;
        case GLFW_KEY_RIGHT_ALT:
            return RIGHT_ALT;
        case GLFW_KEY_LEFT_CONTROL:
            return LEFT_CONTROL;
        case GLFW_KEY_RIGHT_CONTROL:
            return RIGHT_CONTROL;
        case GLFW_KEY_LEFT_SHIFT:
            return LEFT_CONTROL;
        case GLFW_KEY_RIGHT_SHIFT:
            return RIGHT_CONTROL;
        case GLFW_KEY_TAB:
            return TAB;
        case GLFW_KEY_SPACE:
            return SPACE;
        case GLFW_KEY_ESCAPE:
            return ESCAPE;
        case GLFW_KEY_BACKSPACE:
            return BACKSPACE;
        case GLFW_KEY_SLASH:
            return SLASH;
        case GLFW_KEY_BACKSLASH:
            return BACKSLASH;
        case GLFW_KEY_CAPS_LOCK:
            return CAPS_LOCK;
        case GLFW_KEY_ENTER:
            return ENTER;
        case GLFW_KEY_APOSTROPHE:
            return APOSTROPHE;
        case GLFW_KEY_COMMA:
            return COMMA;
        case GLFW_KEY_INSERT:
            return INSERT;
        case GLFW_KEY_DELETE:
            return DELETE;
        case GLFW_KEY_HOME:
            return HOME;
        case GLFW_KEY_END:
            return END;
        case GLFW_KEY_PAGE_UP:
            return PAGE_UP;
        case GLFW_KEY_PAGE_DOWN:
            return PAGE_DOWN;
        case GLFW_KEY_PRINT_SCREEN:
            return PRINT_SCREEN;
        case GLFW_KEY_SCROLL_LOCK:
            return SCROLL_LOCK;
        case GLFW_KEY_PAUSE:
            return PAUSE;
        case GLFW_KEY_UP:
            return UP;
        case GLFW_KEY_DOWN:
            return DOWN;
        case GLFW_KEY_LEFT:
            return LEFT;
        case GLFW_KEY_RIGHT:
            return RIGHT;
        case GLFW_KEY_EQUAL:
            return EQUAL;
        case GLFW_KEY_GRAVE_ACCENT:
            return GRAVE_ACCENT;
        case GLFW_KEY_SEMICOLON:
            return SEMICOLON;
        case GLFW_KEY_MINUS:
            return MINUS;
        case GLFW_KEY_LEFT_BRACKET:
            return LEFT_BRACKET;
        case GLFW_KEY_RIGHT_BRACKET:
            return RIGHT_BRACKET;
        case GLFW_KEY_PERIOD:
            return PERIOD;
        case GLFW_KEY_WORLD_1:
            return WORLD_1;
        case GLFW_KEY_WORLD_2:
            return WORLD_2;
        default:
            LoggerSingleton::Instance().LogWarning("Key '" + std::to_string(key) + "' not mapped!");
            //throw std::exception("Key not mapped");
    }
}

KeyState Application::GetMouseKeyEventByGLFW(const int action)
{
    switch (action)
    {
        case GLFW_PRESS:
        case GLFW_REPEAT:
            return KeyState::Press;
        case GLFW_RELEASE:
            return KeyState::Release;
        default:
            LoggerSingleton::Instance().LogWarning("KeyEvent '" + std::to_string(action) + "' not mapped!");
            //throw std::exception("KeyEvent not implemented!");
    }
}
