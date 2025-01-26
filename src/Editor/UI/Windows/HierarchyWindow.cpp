#include "HierarchyWindow.h"

#include "../../Controllers/EntityController.h"
#include "../../Singletons/EditorSingleton.h"
#include "../../Vendors/imgui/imgui.h"
#include "../../../Core/Loggers/LoggerSingleton.h"
#include "../../../Core/Inputs/Input.h"
#include "../../../Core/Inputs/Key.h"
#include "../../../Core/Inputs/KeyState.h"
#include "ECS/Components/ChildrenComponent.h"
#include "ECS/Components/ParentComponent.h"

using namespace DreamEngine::Editor::Singletons;
using namespace DreamEngine::Editor::UI::Windows;

HierarchyWindow::HierarchyWindow(const std::string& title) : BaseWindow(title) {}

void HierarchyWindow::DrawContent()
{
    EntityManager* entityManager = EditorSingleton::Instance().GetEntityManager();

    if (m_sceneController.ShouldLoadSceneData(m_selectedScenePath))
    {
        m_selectedScenePath = EditorSingleton::Instance().GetSelectedScenePath();
        m_sceneController.LoadSceneData(m_selectedScenePath, entityManager);
    }

    if (!m_selectedScenePath.empty())
    {
        if (ImGui::Button("  Create Entity  "))
        {
            LoggerSingleton::Instance().LogInfo("Add new Entity");
            entityManager->AddEntity("entity");
        }

        const int sceneTreeNodeFlag = EditorSingleton::Instance().IsViewSceneData()
            ? ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen
            : ImGuiTreeNodeFlags_DefaultOpen;

        if (ImGui::TreeNodeEx(m_selectedScenePath.filename().string().c_str(), sceneTreeNodeFlag))
        {
            if (ImGui::IsItemClicked())
                EditorSingleton::Instance().SetIsViewSceneData(true);

            for (Entity* entity : entityManager->GetEntities())
            {
                // continue because it will be render on the parent entity
                if (entity->GetComponent<ParentComponent>().has)
                    continue;

                AddEntityTreeNode(entity);
            }

            ImGui::TreePop();
        }
    }
}

void HierarchyWindow::AddEntityTreeNode(Entity* entity)
{
    string id = entity->GetName() + "##" + std::to_string(entity->GetId());
    const bool isSelectedEntity = entity == EditorSingleton::Instance().GetSelectedEntity() && !EditorSingleton::Instance().IsViewSceneData();
    ChildrenComponent& childrenComponent = entity->GetComponent<ChildrenComponent>();
    const int treeNodeDefault = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth;
    const int treeNodeFlagWithChildren = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
    const int treeNodeFlag = childrenComponent.children.empty() ? treeNodeDefault : treeNodeFlagWithChildren;
    const int entityTreeNodeFlag = isSelectedEntity ? ImGuiTreeNodeFlags_Selected | treeNodeFlag : treeNodeFlag;

    if (ImGui::TreeNodeEx(id.c_str(), entityTreeNodeFlag))
    {
        // left click to select entity
        if (ImGui::IsItemClicked())
        {
            EditorSingleton::Instance().SetSelectedEntity(entity);
            EditorSingleton::Instance().SetIsViewSceneData(false);
        }

        // right click menu
        if (ImGui::BeginPopup((id + "hierarchy.right.click.menu").c_str()))
        {
            if (ImGui::MenuItem("Add Child##hierarchy.right.click.menu"))
                EditorSingleton::Instance().GetEntityManager()->AddEntity("entity", entity);

            ImGui::EndPopup();
        }

        // right click to select entity
        if (ImGui::IsItemClicked(1))
            ImGui::OpenPopup((id + "hierarchy.right.click.menu").c_str());

        // render children
        for (Entity* child : childrenComponent.children)
            AddEntityTreeNode(child);

        // render child
        ImGui::TreePop();
    }

    // delete entity
    if (isSelectedEntity && Core::Inputs::Input::IsPressed(Core::Inputs::Key::DELETE) && m_isFocused)
        EntityController::DeleteEntity(entity);
}
