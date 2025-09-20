#include "PropertyWindow.h"

#include "../../Controllers/EntityController.h"
#include "../../Vendors/imgui/imgui.h"
#include "../../Helpers/ImGuiHelper.h"
#include "../../Singletons/EditorSingleton.h"
#include "../../UI/Views/ColorView.h"
#include "../../UI/Views/MaterialView.h"
#include "../../../Core/Application.h"
#include "../../../Core/ECS/Entity.h"
#include "../../../Core/ECS/Components/DirectionalLightComponent.h"
#include "../../../Core/ECS/Components/MaterialComponent.h"
#include "../../../Core/ECS/Components/ScriptComponent.h"
#include "../../../Core/Render/Factories/MeshFactory.h"
#include "../../../Core/Render/OpenGL/OpenGLMesh.h"
#include "../../../Core/Resources/GlobalResourceManager.h"
#include "../../../Core/EngineDefine.h"
#include "../../Vendors/imgui/imgui_internal.h"

#define NEXT_ROW_VECTOR_3_INPUT_WIDTH 70.0f

using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Render::Factories;
using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::Singletons;
using namespace DreamEngine::Editor::UI::Views;

PropertyWindow::PropertyWindow(const std::string& title) : BaseWindow(title) {}

void PropertyWindow::DrawContent()
{
    const auto selectedEntity = EditorSingleton::Instance().GetSelectedEntity();

    if (EditorSingleton::Instance().IsViewSceneData())
        DrawSceneData();
    else if (selectedEntity != nullptr)
    {
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 105);
        ImGui::BeginGroup();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
        ImGui::Text("Active");
        ImGui::EndGroup();

        ImGui::SameLine();
        bool isActive = selectedEntity->GetIsActive();

        if (ImGui::Checkbox("##selected.entity.active", &isActive))
            selectedEntity->SetActive(!selectedEntity->GetIsActive());

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 30);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));         // Normal red
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));  // Hovered red
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));   // Active red

        if (ImGui::Button("  X  "))
            EntityController::DeleteEntity(selectedEntity);

        ImGui::PopStyleColor(3);

        if (ImGuiHelper::BeginTable("table.selected.entity", 2))
        {
            ImGuiHelper::NextRow("##selected.entity.name", "Name", selectedEntity->GetName());
            ImGuiHelper::NextRow("##selected.entity.tag", "Tag", selectedEntity->GetTag());
            ImGui::EndTable();    
        }

        // TODO: render all components
        DrawTransformComponent(selectedEntity);
        DrawMeshComponent(selectedEntity);
        DrawDirectionalLightComponent(selectedEntity);
        DrawScriptComponent(selectedEntity);
        
        ImGui::Separator();
        DrawAddComponent(selectedEntity);
    }
}

void PropertyWindow::DrawSceneData()
{
    Scene* scene = Core::Application::Instance().GetGame()->GetActiveScene();

    ImGui::Text("Scene Data");
    ImGui::Indent(20.0f);

    ColorView::Draw("Background", *scene->GetBackgroundColor());

    ImGui::Indent(-20.0f);

    if (ImGui::CollapsingHeader("Camera##scene.camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        Camera& camera = scene->GetCamera();

        if (ImGuiHelper::BeginTable("table.scene.camera", 2))
        {
            // position
            ImGuiHelper::PrepareRow("Position");
            ImGui::TextColored(ImGuiHelper::GetImVec4Red(), "X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            ImGui::InputFloat("##scene.camera.position.x", &camera.position.x, 0, 0, "%.2f");

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Green(), "Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            ImGui::InputFloat("##scene.camera.position.y", &camera.position.y, 0, 0, "%.2f");

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Blue(), "Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            ImGui::InputFloat("##scene.camera.position.z", &camera.position.z, 0, 0, "%.2f");

            // front
            ImGuiHelper::PrepareRow("Front");
            ImGui::TextColored(ImGuiHelper::GetImVec4Red(), "X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            ImGui::InputFloat("##scene.camera.front.x", &camera.front.x, 0, 0, "%.2f");

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Green(), "Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            ImGui::InputFloat("##scene.camera.front.y", &camera.front.y, 0, 0, "%.2f");

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Blue(), "Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            ImGui::InputFloat("##scene.camera.front.z", &camera.front.z, 0, 0, "%.2f");

            // up
            ImGuiHelper::PrepareRow("Up");
            ImGui::TextColored(ImGuiHelper::GetImVec4Red(), "X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            float upX = std::isnan(camera.up.x) ? 0.0f : camera.up.x;
            ImGui::InputFloat("##scene.camera.up.x", &upX, 0, 0, "%.2f");
            camera.up.x = upX;

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Green(), "Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            float upY = std::isnan(camera.up.y) ? 0.0f : camera.up.y;
            ImGui::InputFloat("##scene.camera.up.y", &upY, 0, 0, "%.2f");
            camera.up.y = upY;

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Blue(), "Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            float upZ = std::isnan(camera.up.z) ? 0.0f : camera.up.z;
            ImGui::InputFloat("##scene.camera.up.z", &upZ, 0, 0, "%.2f");
            camera.up.z = upZ;

            ImGuiHelper::PrepareRow("Fov");
            ImGui::InputFloat("##scene.camera.fov", &camera.fovDegree, 0, 0, "%.2f");

            ImGuiHelper::PrepareRow("Near");
            ImGui::InputFloat("##scene.camera.near", &camera.near, 0, 0, "%.2f");

            ImGuiHelper::PrepareRow("Far");
            ImGui::InputFloat("##scene.camera.far", &camera.far, 0, 0, "%.2f");

            ImGui::EndTable();
        }
    }


    if (ImGui::CollapsingHeader("Global Light##scene.global.light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Indent(20.0f);

        GlobalLight* globalLight = scene->GetGlobalLight();

        DrawDirectionalLightComponent(globalLight->directionalLight);
        DrawTransformComponent(globalLight->transform);

        ImGui::Indent(-20.0f);
    }

    // TODO: render name as name: something scene
    /*ImGui::TableNextRow();
            ImGui::TableNextColumn();
            */
}

void PropertyWindow::DrawTransformComponent(Entity* selectedEntity)
{
    if (selectedEntity == nullptr)
        return;

    TransformComponent& transformComponent = selectedEntity->GetComponent<TransformComponent>();

    DrawTransformComponent(transformComponent);
}

void PropertyWindow::DrawTransformComponent(TransformComponent& transform)
{
    if (ImGui::CollapsingHeader("Transform##selected.entity.transform.component", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGuiHelper::BeginTable("table.selected.entity.transform.component", 2))
        {
            // position
            glm::vec3 position = transform.GetPosition();
            ImGuiHelper::PrepareRow("Position");
            ImGui::TextColored(ImGuiHelper::GetImVec4Red(), "X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            if (ImGui::InputFloat("##selected.entity.position.x", &position.x, 0, 0, "%.2f"))
                transform.SetPosition(position);

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Green(), "Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            if (ImGui::InputFloat("##selected.entity.position.y", &position.y, 0, 0, "%.2f"))
                transform.SetPosition(position);

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Blue(), "Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            if (ImGui::InputFloat("##selected.entity.position.z", &position.z, 0, 0, "%.2f"))
                transform.SetPosition(position);

            // scale
            glm::vec3 scale = transform.GetScale();
            ImGuiHelper::PrepareRow("Scale");
            ImGui::TextColored(ImGuiHelper::GetImVec4Red(), "X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            if (ImGui::InputFloat("##selected.entity.scale.x", &scale.x, 0, 0, "%.2f"))
                transform.SetScale(scale);

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Green(), "Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            if (ImGui::InputFloat("##selected.entity.scale.y", &scale.y, 0, 0, "%.2f"))
                transform.SetScale(scale);

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Blue(), "Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            if (ImGui::InputFloat("##selected.entity.scale.z", &scale.z, 0, 0, "%.2f"))
                transform.SetScale(scale);

            // rotation
            glm::vec3 rotation = transform.GetRotation();
            ImGuiHelper::PrepareRow("Rotation");
            ImGui::TextColored(ImGuiHelper::GetImVec4Red(), "X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            if (ImGui::InputFloat("##selected.entity.rotation.x", &rotation.x, 0, 0, "%.2f"))
                transform.SetRotation(rotation);

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Green(), "Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            if (ImGui::InputFloat("##selected.entity.rotation.y", &rotation.y, 0, 0, "%.2f"))
                transform.SetRotation(rotation);

            ImGui::SameLine();
            ImGui::TextColored(ImGuiHelper::GetImVec4Blue(), "Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(NEXT_ROW_VECTOR_3_INPUT_WIDTH);
            if (ImGui::InputFloat("##selected.entity.rotation.z", &rotation.z, 0, 0, "%.2f"))
                transform.SetRotation(rotation);

            ImGui::EndTable();
        }
    }
}

void PropertyWindow::DrawMeshComponent(Entity* selectedEntity)
{
    if (selectedEntity == nullptr)
        return;

    MeshComponent& meshComponent = selectedEntity->GetComponent<MeshComponent>();

    if (!meshComponent.has)
        return;

    if (ImGui::CollapsingHeader("Mesh##selected.entity.mesh.component", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGuiHelper::BeginTable("mesh.view.table", 2))
        {
            ImGuiHelper::PrepareRow("Mesh");

            std::string meshName = meshComponent.mesh == nullptr ? "" : meshComponent.mesh->name;

            if (ImGui::BeginCombo("##mesh.view.combo", meshName.c_str()))
            {
                for (auto& [_, pMesh] : GlobalResourceManager::Instance().GetMeshes())
                {
                    const bool isSelected = meshName == pMesh->name;

                    if (ImGui::Selectable(pMesh->name.c_str(), isSelected))
                        meshComponent.mesh = pMesh;

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            ImGui::EndTable();
        }

        // TODO: show the meshes here
        DrawMaterialComponent(selectedEntity);
    }
}

void PropertyWindow::DrawMaterialComponent(Entity* selectedEntity)
{
    if (selectedEntity == nullptr)
        return;

    MaterialComponent& materialComponent = selectedEntity->GetComponent<MaterialComponent>();

    if (!materialComponent.has)
        return;

     m_materialView.Draw(*materialComponent.material, [this](Material* mat)
     {
         MaterialComponent& materialComponent = EditorSingleton::Instance().GetSelectedEntity()->GetComponent<MaterialComponent>();

         if (materialComponent.has)
             materialComponent.material = mat;
     });
}

void PropertyWindow::DrawScriptComponent(Entity* selectedEntity)
{

    if (selectedEntity == nullptr)
        return;

    ScriptComponent& scriptComponent = selectedEntity->GetComponent<ScriptComponent>();

    if (!scriptComponent.has)
        return;

    if (ImGui::CollapsingHeader("Script##selected.entity.script.component", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGuiHelper::BeginTable("selected.entity.script.component.table", 2))
        {
            string name = scriptComponent.script != nullptr ? scriptComponent.script->name : "";

            ImGuiHelper::NextRow("##script.component.name", "Name", name, false);

            if (ImGui::Button("Set Script##script.component.set.script", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                ImGui::OpenPopup("script.component.set.script.popup");
            }

            if (ImGui::BeginPopup("script.component.set.script.popup"))
            {
                for (auto pairScript : GlobalResourceManager::Instance().GetScripts())
                {
                    if (ImGui::Selectable(pairScript.second->name.c_str()))
                    {
                        scriptComponent.script = pairScript.second;
                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::EndPopup();
            }

            ImGui::EndTable();
        }
    }
}

void PropertyWindow::DrawAddComponent(Entity* selectedEntity)
{
    if (selectedEntity == nullptr)
        return;

    if (ImGui::Button("    Add Component    "))
        ImGui::OpenPopup("Add Component##selected.entity.add.component.popup");

    if (ImGui::BeginPopup("Add Component##selected.entity.add.component.popup", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking))
    {
        MeshComponent& meshComponent = selectedEntity->GetComponent<MeshComponent>();
        MaterialComponent& materialComponent = selectedEntity->GetComponent<MaterialComponent>();
        ScriptComponent& scriptComponent = selectedEntity->GetComponent<ScriptComponent>();

        // mesh
        if (!meshComponent.has)
        {
            if (ImGui::Button("Mesh Component##selected.entity.add.cube.mesh.component.button"))
                ImGui::OpenPopup("Cube##selected.entity.add.cube.mesh.component.popup");

            if (ImGui::BeginPopup("Cube##selected.entity.add.cube.mesh.component.popup"))
            {
                if (ImGui::MenuItem("Empty##selected.entity.add.empty.mesh.component"))
                {
                    meshComponent.has = true;

                    if (!materialComponent.has)
                    {
                        materialComponent.has = true;
                        materialComponent.material = GlobalResourceManager::Instance().GetMaterial(DEFAULT_MATERIAL_NAME);
                    }

                    ImGui::ClosePopupsExceptModals();
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Cube##selected.entity.add.cube.mesh.component"))
                {
                    meshComponent.has = true;
                    meshComponent.mesh = GlobalResourceManager::Instance().GetMesh(DEFAULT_CUBE_MESH_NAME);

                    if (!materialComponent.has)
                    {
                        materialComponent.has = true;
                        materialComponent.material = GlobalResourceManager::Instance().GetMaterial(DEFAULT_MATERIAL_NAME);
                    }

                    ImGui::ClosePopupsExceptModals();
                }

                if (ImGui::MenuItem("Sphere##selected.entity.add.sphere.mesh.component"))
                {
                    meshComponent.has = true;
                    meshComponent.mesh = GlobalResourceManager::Instance().GetMesh(DEFAULT_SPHERE_MESH_NAME);

                    if (!materialComponent.has)
                    {
                        materialComponent.has = true;
                        materialComponent.material = GlobalResourceManager::Instance().GetMaterial(DEFAULT_MATERIAL_NAME);
                    }

                    ImGui::ClosePopupsExceptModals();
                }

                if (ImGui::MenuItem("Capsule##selected.entity.add.capsule.mesh.component"))
                {
                    meshComponent.has = true;
                    meshComponent.mesh = GlobalResourceManager::Instance().GetMesh(DEFAULT_CAPSULE_MESH_NAME);

                    if (!materialComponent.has)
                    {
                        materialComponent.has = true;
                        materialComponent.material = GlobalResourceManager::Instance().GetMaterial(DEFAULT_MATERIAL_NAME);
                    }

                    ImGui::ClosePopupsExceptModals();
                }

                if (ImGui::MenuItem("Cylinder##selected.entity.add.cylinder.mesh.component"))
                {
                    meshComponent.has = true;
                    meshComponent.mesh = GlobalResourceManager::Instance().GetMesh(DEFAULT_CYLINDER_MESH_NAME);

                    if (!materialComponent.has)
                    {
                        materialComponent.has = true;
                        materialComponent.material = GlobalResourceManager::Instance().GetMaterial(DEFAULT_MATERIAL_NAME);
                    }

                    ImGui::ClosePopupsExceptModals();
                }

                if (ImGui::MenuItem("Plane##selected.entity.add.plane.mesh.component"))
                {
                    meshComponent.has = true;
                    meshComponent.mesh = GlobalResourceManager::Instance().GetMesh(DEFAULT_PLANE_MESH_NAME);

                    if (!materialComponent.has)
                    {
                        materialComponent.has = true;
                        materialComponent.material = GlobalResourceManager::Instance().GetMaterial(DEFAULT_MATERIAL_NAME);
                    }

                    ImGui::ClosePopupsExceptModals();
                }

                ImGui::Separator();

                // close mesh component popup
                /*if (ImGui::MenuItem("Close##selected.entity.add.cube.mesh.component.popup.close.button"))
                    ImGui::CloseCurrentPopup();*/

                ImGui::EndPopup();
            }
        //    ImGui::CloseCurrentPopup();
        }

        // script
        if (!scriptComponent.has)
        {
            if (ImGui::MenuItem("Script Component##selected.entity.add.script.component.button"))
            {
                scriptComponent.has = true;
                ImGui::CloseCurrentPopup();
            }
        }

        // close main popup
        /*if (ImGui::MenuItem("Close##selected.entity.add.component.popup.close.button"))
            ImGui::CloseCurrentPopup();*/

        ImGui::EndPopup();
    }
}

void PropertyWindow::DrawDirectionalLightComponent(Entity* selectedEntity)
{
    if (selectedEntity == nullptr)
        return;

    DirectionalLightComponent& directionLightComponent = selectedEntity->GetComponent<DirectionalLightComponent>();

    if (directionLightComponent.has)
        DrawDirectionalLightComponent(directionLightComponent);
}

void PropertyWindow::DrawDirectionalLightComponent(DirectionalLightComponent& directionLight)
{
    if (ImGui::CollapsingHeader("Direction Light##selected.entity.direction.light.component", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ColorView::Draw("Color", directionLight.color);
        ColorView::Draw("Specular", directionLight.specular);

        if (ImGuiHelper::BeginTable("table.selected.entity.direction.light.component.influence", 2))
        {
            ImGuiHelper::PrepareRow("Influence");
            ImGui::InputFloat("##", &directionLight.influence);
            ImGui::EndTable();   
        }
    }
}
