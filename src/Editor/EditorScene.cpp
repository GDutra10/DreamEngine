#include "EditorScene.h"

#include "Helpers/FileHelper.h"
#include "Serializers/MaterialSerializer.h"
#include "Singletons/EditorSingleton.h"
#include "Vendors/imgui/imgui.h"
#include "Vendors/imgui/imgui_impl_glfw.h"
#include "Vendors/imgui/imgui_impl_opengl3.h"
#include "Vendors/imgui/imgui_internal.h"
#include "Vendors/imgui/ImGuizmo.h"
#include "Vendors/stb_image.h"
#include "../Core/Application.h"
#include "../Core/Loggers/LoggerSingleton.h"
#include "../Core/Render/Factories/MeshFactory.h"
#include "../Core/Inputs/Input.h"
#include "../Core/IO/File.h"
#include "Render/RenderViewProvider.h"


using namespace DreamEngine::Editor;
using namespace DreamEngine::Editor::Serializers;
using namespace DreamEngine::Editor::Singletons;
using namespace DreamEngine::Core;
using namespace DreamEngine::Core::IO;
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Render::Factories;


#ifndef EDITOR_FONT_PATH
#define EDITOR_FONT_PATH "Assets/Fonts/Roboto-Regular.ttf"
#endif

EditorScene::EditorScene(
    std::string name,
    ProjectConfiguration& projectConfig,
    EditorConfiguration& editorConfig)
    : Scene(std::move(name))
      , m_gameWindow(GameWindow("Game"))
      , m_hierarchyWindow(HierarchyWindow("Hierarchy"))
      , m_loggerWindow(LoggerWindow("Output"))
      , m_propertyWindow(PropertyWindow("Property"))
      , m_projectWindow(ProjectWindow("Project"))
      , m_sceneWindow(SceneWindow("Scene"))
      , m_fileExplorerWindow(FileExplorerWindow("File Explorer"))
      , m_resourceManagerWindow(ResourceManagerWindow("Resource Manager"))
      , m_materialWindow(MaterialWindow("Material"))
      , m_openProjectModal(OpenProjectModal("Open Project"))
{
    EditorSingleton::Initialize(projectConfig, editorConfig, this);
}

void EditorScene::Initialize()
{
    Scene::Initialize();
    InitializeImGui();

    Application::Instance().GetRenderAPI()->AddBeforeRenderEntitiesCallbacks([this](RenderView& renderView) { UpdateBackgroundColor(renderView); });
    Application::Instance().GetRenderAPI()->AddBeforeRenderEntitiesCallbacks([this](RenderView& renderView) { StartImGuiFrame(renderView); });
    Application::Instance().GetRenderAPI()->AddAfterRenderEntitiesCallbacks([this](RenderView& renderView) { FinishImGuiFrame(renderView); });
    
    EditorSingleton::Instance().SetEntityManager(m_entityManager);
    
    Game* game = Application::Instance().GetGame();
    
    // create font
    Font* defaultFont = new Font(Helpers::FileHelper::LoadFileIntoVector(EDITOR_FONT_PATH));
    defaultFont->path = EDITOR_FONT_PATH;
    ResourceManager::Instance().AddFont("default_font", defaultFont);
    UiManager::AddFont(defaultFont);

    // add viewport
    FrameBuffer* viewportFbo = Application::Instance().GetRenderAPI()->CreateFrameBuffer(game->width, game->height);
    auto viewportRenderView = new RenderView();
    viewportRenderView->mask = RenderMask::World | RenderMask::Debug;
    viewportRenderView->frameBuffer = viewportFbo;
    
    EditorSingleton::Instance().SetSceneRenderView(viewportRenderView);

    // add game viewport
    FrameBuffer* gameViewportFbo = Application::Instance().GetRenderAPI()->CreateFrameBuffer(game->width, game->height);
    auto gameViewportRenderView = new RenderView();
    gameViewportRenderView->mask = RenderMask::World | RenderMask::UI;
    gameViewportRenderView->frameBuffer = gameViewportFbo;

    EditorSingleton::Instance().SetGameRenderView(gameViewportRenderView);

    // initialize scripts without running them
    m_mustRunScriptComponents = false;

    // configure stb_image to flip loaded texture's on the y-axis
    stbi_set_flip_vertically_on_load(true);

    EditorSingleton::Instance().GetProjectController().LoadProjectConfiguration();
}

void EditorScene::Unload()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EditorScene::Update(const float deltaTime)
{
    Scene::Update(deltaTime);
    EditorSingleton::Instance().GetCameraEditorController().Update(m_sceneWindow.IsFocused());
}

bool EditorScene::GetIsFocused() const
{
    return m_gameWindow.IsFocused();
}

Camera& EditorScene::GetCamera()
{
    RenderView* sceneRenderView= EditorSingleton::Instance().GetSceneRenderView();

    if (sceneRenderView->frameBuffer->GetIsActive())
        return *EditorSingleton::Instance().GetCameraEditorController().GetCamera();

    return Scene::GetCamera();
}

ProjectConfiguration& EditorScene::GetProjectConfiguration() const
{
    return EditorSingleton::Instance().GetProjectConfiguration();
}

void EditorScene::InitializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // configure dock
    m_io = &ImGui::GetIO();
    (void)m_io;
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // configure fonts
    m_io->Fonts->AddFontFromFileTTF(EDITOR_FONT_PATH, 16.0f);

    // styles
    ImGui::StyleColorsDark();
    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        SetStyleEngine();

    // init openGL
    const auto app = Core::Application::Instance();

    assert(app.GetWindow() != nullptr);

    ImGui_ImplGlfw_InitForOpenGL(app.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

void EditorScene::DrawMenuBar()
{
    ProjectConfiguration& projectConfiguration = GetProjectConfiguration();

    // Create a menu bar
    bool mustOpenProjectModal = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open Project", "Ctrl+O"))
            {
                mustOpenProjectModal = true;
            }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S", false, projectConfiguration.isLoaded))
            {
                SceneController::SaveSceneData(m_entityManager);
            }
            if (ImGui::MenuItem("Exit", "Alt+F4"))
            {
                glfwSetWindowShouldClose(Core::Application::Instance().GetWindow(), GLFW_TRUE);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit", projectConfiguration.isLoaded))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            {
                // Handle Undo action
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y"))
            {
                // Handle Redo action
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Assets", projectConfiguration.isLoaded))
        {
            if (ImGui::MenuItem("Recompile Scripts"))
                EditorSingleton::Instance().GetScriptController().ReloadScripts();

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Default"))
            {
                SetStyleEngine();
            }
            if (ImGui::MenuItem("Style 1"))
            {
                SetStyleOne();
            }
            if (ImGui::MenuItem("Style Unreal"))
            {
                SetStyleUnreal();
            }
            if (ImGui::MenuItem("Style Unreal 2"))
            {
                SetStyleUnreal2();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window", projectConfiguration.isLoaded))
        {
            if (ImGui::MenuItem("Editor"))
            {
                // Handle About action
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                // Handle About action
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (mustOpenProjectModal)
        m_openProjectModal.Open();

    // modals
    m_openProjectModal.Draw();
}

void EditorScene::DrawTopBar()
{
    const bool mustRunScriptComponents = EditorSingleton::Instance().GetEditorScene()->GetMustRunScriptComponents();
    const char* label = mustRunScriptComponents ? " Stop " : " Play ";
    const ImGuiStyle& style = ImGui::GetStyle();
    const ImVec4 base = mustRunScriptComponents ? ImVec4(0.957f, 0.263f, 0.212f, 1.0f) : ImVec4(0.298f, 0.686f, 0.314f, 1.0f);    // normal
    const ImVec4 hover = mustRunScriptComponents ? ImVec4(0.937f, 0.325f, 0.314f, 1.0f) : ImVec4(0.400f, 0.733f, 0.416f, 1.0f);   // hovered
    const ImVec4 active = mustRunScriptComponents ? ImVec4(0.898f, 0.224f, 0.208f, 1.0f) : ImVec4(0.263f, 0.627f, 0.278f, 1.0f);  // active (pressed)

    ImGui::PushStyleColor(ImGuiCol_WindowBg, style.Colors[ImGuiCol_MenuBarBg]);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.FramePadding.y));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, style.FrameBorderSize);
    ImGui::PushStyleColor(ImGuiCol_Button, base);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);

    if (ImGui::BeginViewportSideBar(
            "##TopToolbar", 
            ImGui::GetMainViewport(),
            ImGuiDir_Up,
            ImGui::GetFrameHeight() * 1.6f,  // example taller bar
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
    {
        // --- compute button size (default button size uses frame height) ---
        const float buttonWidth = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
        const float buttonHeight = ImGui::GetFrameHeight();

        // --- content region size (excludes padding/borders) ---
        const float innterWidht = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
        const float innerHeight = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;

        // --- starting cursor inside content region ---
        const ImVec2 start = ImGui::GetCursorStartPos();

        // --- center offsets ---
        const float offX = std::max(0.0f, (innterWidht - buttonWidth) * 0.5f);
        const float offY = std::max(0.0f, (innerHeight - buttonHeight) * 0.5f);

        // place cursor at centered position
        ImGui::SetCursorPos(ImVec2(start.x + offX, start.y + offY));

        if (ImGui::Button(label))
            mustRunScriptComponents ? SceneController::Stop(m_entityManager) : SceneController::Play(m_entityManager);
    }
    ImGui::End();

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(4);
}

void EditorScene::StartImGuiFrame(const RenderView& renderView)
{
    if (!renderView.IsDefault())
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void EditorScene::FinishImGuiFrame(const RenderView& renderView)
{
    if (!renderView.IsDefault())
        return;

    ImGui::DockSpaceOverViewport();

    DrawMenuBar();

    if (GetProjectConfiguration().isLoaded)
    {
        DrawTopBar();
        m_hierarchyWindow.Draw();
        m_loggerWindow.Draw();
        m_propertyWindow.Draw();
        m_projectWindow.Draw();
        m_sceneWindow.Draw();
        m_gameWindow.Draw();
        m_fileExplorerWindow.Draw();
        m_resourceManagerWindow.Draw();
        m_materialWindow.Draw();    
    }

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

// update the scene background by scene config data
void EditorScene::UpdateBackgroundColor(const RenderView& renderView) const
{
    if (!renderView.IsDefault())
        return;

    if (const auto sceneData = EditorSingleton::Instance().sceneData; sceneData != nullptr)
    {
        const auto backgroundColor = this->GetBackgroundColor();
        backgroundColor->red = backgroundColor->red;
        backgroundColor->green = backgroundColor->green;
        backgroundColor->blue = backgroundColor->blue;
        backgroundColor->alpha = backgroundColor->alpha;
    }
}

void EditorScene::SetStyleOne()
{
    //style.WindowRounding = 0.0f;
    //style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    ImGuiStyle* style = &ImGui::GetStyle();
    style->ScaleAllSizes(1);

    style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}

void EditorScene::SetStyleUnreal()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowPadding = ImVec2(8.0f, 8.0f);
    style->FramePadding = ImVec2(5.0f, 2.0f);
    style->ItemSpacing = ImVec2(6.0f, 6.0f);
    style->ItemInnerSpacing = ImVec2(6.0f, 6.0f);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->GrabMinSize = 5.0f;

    style->WindowRounding = 0.0f;
    style->FrameRounding = 0.0f;
    style->PopupRounding = 0.0f;
    style->ScrollbarRounding = 0.0f;
    style->GrabRounding = 0.0f;
    style->TabRounding = 0.0f;

    style->WindowBorderSize = 1.0f;
    style->ChildBorderSize = 1.0f;
    style->PopupBorderSize = 1.0f;
    style->FrameBorderSize = 1.0f;
    style->TabBorderSize = 1.0f;

    style->Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style->Colors[ImGuiCol_Separator] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style->Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_TabActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.86f, 0.86f, 0.86f, 0.70f);
    style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.60f, 0.00f, 0.90f);
    style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.35f);

    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style->WindowRounding = 0.0f;
        style->Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

void EditorScene::SetStyleUnreal2()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Set color palette to mimic Unreal Engine's UI style
    colors[ImGuiCol_Text] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.98f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 0.75f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.75f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.30f, 0.30f, 0.60f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.75f, 0.75f, 0.75f, 0.60f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.85f, 0.85f, 0.90f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.77f, 0.33f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.96f, 0.88f, 0.56f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.85f, 0.85f, 0.85f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.35f);

    // Adjust style parameters
    style.WindowRounding = 2.0f;
    style.FrameRounding = 2.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 2.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.FramePadding = ImVec2(5.0f, 3.0f);
    style.ItemSpacing = ImVec2(6.0f, 6.0f);
}

void EditorScene::SetStyleEngine()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    auto mainBgColor = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 1.00f);
    auto modalBgColor = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 0.40f);
    auto childBgColor = ImVec4(44.0f / 255.0f, 44.0f / 255.0f, 44.0f / 255.0f, 1.00f);

    // Set color palette to mimic Unreal Engine's UI style
    colors[ImGuiCol_Text] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = childBgColor;
    colors[ImGuiCol_ChildBg] = mainBgColor;
    colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.98f);
    colors[ImGuiCol_Border] = mainBgColor;
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = mainBgColor;
    colors[ImGuiCol_FrameBgHovered] = mainBgColor;
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    colors[ImGuiCol_TitleBg] = mainBgColor;
    colors[ImGuiCol_TitleBgActive] = mainBgColor;
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 0.75f);
    colors[ImGuiCol_MenuBarBg] = mainBgColor;
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.75f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.30f, 0.30f, 0.60f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.75f, 0.75f, 0.75f, 0.60f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.85f, 0.85f, 0.90f);
    colors[ImGuiCol_Tab] = mainBgColor;
    colors[ImGuiCol_TabHovered] = childBgColor;
    colors[ImGuiCol_TabActive] = childBgColor;
    colors[ImGuiCol_TabUnfocused] = mainBgColor;
    colors[ImGuiCol_TabUnfocusedActive] = childBgColor;
    colors[ImGuiCol_DockingPreview] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.77f, 0.33f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.96f, 0.88f, 0.56f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.85f, 0.85f, 0.85f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = modalBgColor;

    // Adjust style parameters
    style.WindowRounding = 0.0f;
    style.FrameRounding = 2.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 8.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;
    style.WindowPadding = ImVec2(2.0f, 8.0f);
    style.FramePadding = ImVec2(0.0f, 5.0f);
    style.ItemSpacing = ImVec2(6.0f, 6.0f);
    style.WindowMenuButtonPosition = ImGuiDir_None;
}