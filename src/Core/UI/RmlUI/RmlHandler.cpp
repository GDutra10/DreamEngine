
#include "RmlHandler.h"

#include "CustomRenderInterface_GL3.h"
#include "Loggers/LoggerSingleton.h"
#include "RmlUi_Platform_GLFW.h"
#include "RmlUi_Renderer_GL3.h"
#include "RmlUi/Core/Core.h"
#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/Log.h"

using namespace DreamEngine::Core::UI::RmlUI;

Rml::Context* RmlHandler::m_spContext = nullptr;
SystemInterface_GLFW* RmlHandler::m_spSystemInterface = nullptr;
CustomRenderInterface_GL3* RmlHandler::m_spRenderInterface = nullptr;

void RmlHandler::Initialize(GLFWwindow* window, int width, int height)
{
    Loggers::LoggerSingleton::Instance().LogTrace("RmlHandler::Initialize -> Start");

    glfwMakeContextCurrent(window);

    m_spSystemInterface = new SystemInterface_GLFW();
    m_spRenderInterface = new CustomRenderInterface_GL3();

    Rml::SetRenderInterface(m_spRenderInterface);
    Rml::SetSystemInterface(m_spSystemInterface);

    if (!Rml::Initialise())
    {
        ShutdownInterfaces();

        throw std::runtime_error("Rml::Initialise() failed");
    }

    m_spContext = Rml::CreateContext("main", Rml::Vector2i(width, height));

    if (!m_spContext)
    {
        Rml::Shutdown();
        ShutdownInterfaces();

        throw std::runtime_error("Rml::CreateContext() failed");
    }

    m_spRenderInterface->SetViewport(width, height);

    // TODO: must change to pass via scene!
    const bool isFontOk = ::Rml::LoadFontFace("Assets/Fonts/Roboto-Regular.ttf");

    if (!isFontOk)
        Loggers::LoggerSingleton::Instance().LogWarning("Rml font load failed: Assets/Fonts/Roboto-Regular.ttf");
}

void* RmlHandler::Create(const UiContent* content)
{
    const Rml::String strRml(content->text);
    Rml::ElementDocument* document = m_spContext->LoadDocumentFromMemory(strRml);

    if (document)
        document->Show();

    return document;
}

void RmlHandler::Update()
{
    m_spContext->Update();
}

void RmlHandler::RemoveContents()
{
    m_spContext->UnloadAllDocuments();
}

void RmlHandler::BeginRender(Game* game)
{
    if (!m_spContext || !m_spRenderInterface)
        return;

    m_spRenderInterface->SetViewport(game->width, game->height);
    m_spRenderInterface->BeginFrame();
}

void RmlHandler::Render(Game* game)
{
    if (!m_spContext)
        return;

    m_spContext->Render();
}

void RmlHandler::EndRender()
{
    if (!m_spRenderInterface)
        return;

    m_spRenderInterface->EndFrame();
}

void RmlHandler::Shutdown()
{
    if (m_spContext)
        m_spContext = nullptr;

    Rml::Shutdown();
    ShutdownInterfaces();
}

bool RmlHandler::ProcessMouseMove(const int x, const int y)
{
    if (!m_spContext)
        return false;

    m_spContext->ProcessMouseMove(x, y, 0);

    return m_spContext->GetHoverElement() != nullptr;
}

bool RmlHandler::ProcessMouseButton(const int button, const int action, const int mods)
{
    if (!m_spContext)
        return false;

    int rmlButton = ConvertMouseButton(button);

    if (action == GLFW_PRESS)
    {
        m_spContext->ProcessMouseButtonDown(rmlButton, 0);
    }
    else if (action == GLFW_RELEASE)
    {
        m_spContext->ProcessMouseButtonUp(rmlButton, 0);
    }

    return m_spContext->GetHoverElement() != nullptr;
}

bool RmlHandler::ProcessKey(const int key, const int scancode, const int action, const int mods)
{
    if (!m_spContext)
        return false;

    Rml::Input::KeyIdentifier rmlKey = ConvertKey(key);

    if (rmlKey == Rml::Input::KI_UNKNOWN)
        return false;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        m_spContext->ProcessKeyDown(rmlKey, 0);

        // Handle text input for printable characters
        if (key >= 32 && key <= 126)
        {
            m_spContext->ProcessTextInput(static_cast<Rml::Character>(key));
        }
    }
    else if (action == GLFW_RELEASE)
    {
        m_spContext->ProcessKeyUp(rmlKey, 0);
    }

    // Consider key consumed if any text input field has focus
    Rml::Element* focusElement = m_spContext->GetFocusElement();
    return focusElement != nullptr && (focusElement->GetTagName() == "input" || focusElement->GetTagName() == "textarea");
}

bool RmlHandler::ProcessScroll(const double xOffset, const double yOffset)
{
    if (!m_spContext)
        return false;

    m_spContext->ProcessMouseWheel(static_cast<float>(-yOffset), 0);

    // Check if mouse is over any UI element
    return m_spContext->GetHoverElement() != nullptr;
}

void RmlHandler::ShutdownInterfaces()
{
    delete m_spRenderInterface;
    delete m_spSystemInterface;
    m_spRenderInterface = nullptr;
    m_spSystemInterface = nullptr;
}

Rml::Input::KeyIdentifier RmlHandler::ConvertKey(const int glfwKey)
{
    switch (glfwKey)
    {
        case GLFW_KEY_SPACE:
            return Rml::Input::KI_SPACE;
        case GLFW_KEY_0:
            return Rml::Input::KI_0;
        case GLFW_KEY_1:
            return Rml::Input::KI_1;
        case GLFW_KEY_2:
            return Rml::Input::KI_2;
        case GLFW_KEY_3:
            return Rml::Input::KI_3;
        case GLFW_KEY_4:
            return Rml::Input::KI_4;
        case GLFW_KEY_5:
            return Rml::Input::KI_5;
        case GLFW_KEY_6:
            return Rml::Input::KI_6;
        case GLFW_KEY_7:
            return Rml::Input::KI_7;
        case GLFW_KEY_8:
            return Rml::Input::KI_8;
        case GLFW_KEY_9:
            return Rml::Input::KI_9;
        case GLFW_KEY_A:
            return Rml::Input::KI_A;
        case GLFW_KEY_B:
            return Rml::Input::KI_B;
        case GLFW_KEY_C:
            return Rml::Input::KI_C;
        case GLFW_KEY_D:
            return Rml::Input::KI_D;
        case GLFW_KEY_E:
            return Rml::Input::KI_E;
        case GLFW_KEY_F:
            return Rml::Input::KI_F;
        case GLFW_KEY_G:
            return Rml::Input::KI_G;
        case GLFW_KEY_H:
            return Rml::Input::KI_H;
        case GLFW_KEY_I:
            return Rml::Input::KI_I;
        case GLFW_KEY_J:
            return Rml::Input::KI_J;
        case GLFW_KEY_K:
            return Rml::Input::KI_K;
        case GLFW_KEY_L:
            return Rml::Input::KI_L;
        case GLFW_KEY_M:
            return Rml::Input::KI_M;
        case GLFW_KEY_N:
            return Rml::Input::KI_N;
        case GLFW_KEY_O:
            return Rml::Input::KI_O;
        case GLFW_KEY_P:
            return Rml::Input::KI_P;
        case GLFW_KEY_Q:
            return Rml::Input::KI_Q;
        case GLFW_KEY_R:
            return Rml::Input::KI_R;
        case GLFW_KEY_S:
            return Rml::Input::KI_S;
        case GLFW_KEY_T:
            return Rml::Input::KI_T;
        case GLFW_KEY_U:
            return Rml::Input::KI_U;
        case GLFW_KEY_V:
            return Rml::Input::KI_V;
        case GLFW_KEY_W:
            return Rml::Input::KI_W;
        case GLFW_KEY_X:
            return Rml::Input::KI_X;
        case GLFW_KEY_Y:
            return Rml::Input::KI_Y;
        case GLFW_KEY_Z:
            return Rml::Input::KI_Z;
        case GLFW_KEY_ESCAPE:
            return Rml::Input::KI_ESCAPE;
        case GLFW_KEY_ENTER:
            return Rml::Input::KI_RETURN;
        case GLFW_KEY_TAB:
            return Rml::Input::KI_TAB;
        case GLFW_KEY_BACKSPACE:
            return Rml::Input::KI_BACK;
        case GLFW_KEY_LEFT:
            return Rml::Input::KI_LEFT;
        case GLFW_KEY_RIGHT:
            return Rml::Input::KI_RIGHT;
        case GLFW_KEY_UP:
            return Rml::Input::KI_UP;
        case GLFW_KEY_DOWN:
            return Rml::Input::KI_DOWN;
        case GLFW_KEY_LEFT_SHIFT:
            return Rml::Input::KI_LSHIFT;
        case GLFW_KEY_RIGHT_SHIFT:
            return Rml::Input::KI_RSHIFT;
        case GLFW_KEY_LEFT_CONTROL:
            return Rml::Input::KI_LCONTROL;
        case GLFW_KEY_RIGHT_CONTROL:
            return Rml::Input::KI_RCONTROL;
        default:
            return Rml::Input::KI_UNKNOWN;
    }
}

int RmlHandler::ConvertMouseButton(const int glfwButton)
{
    switch (glfwButton)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            return 0;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return 1;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return 2;
        default:
            return 0;
    }
}

int RmlHandler::GetKeyModifiers(GLFWwindow* window)
{
    int modifiers = 0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        modifiers |= Rml::Input::KM_SHIFT;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        modifiers |= Rml::Input::KM_CTRL;
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
        modifiers |= Rml::Input::KM_ALT;
    return modifiers;
}