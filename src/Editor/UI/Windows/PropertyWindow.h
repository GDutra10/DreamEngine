#ifndef EDITOR_UI_WINDOWS_PROPERTY_WINDOW_H_
#define EDITOR_UI_WINDOWS_PROPERTY_WINDOW_H_

#include "BaseWindow.h"
#include "../Views/MaterialView.h"
#include "../../Controllers/EntityController.h"
#include "../../Controllers/ResourceController.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::UI::Views;
using namespace DreamEngine::Editor::Controllers;

class PropertyWindow : public BaseWindow
{
   public:
    PropertyWindow(const std::string& title, EditorContext& editorContext, EntityController& entityController, ResourceController& resourceController);

   protected:
    void DrawContent() override;

    private:
    EntityController& m_entityController;
    ResourceController& m_resourceController;
    MaterialView m_materialView;
    static void DrawSceneData();
    // Draw components
    static void DrawDirectionalLightComponent(Entity* selectedEntity);
    static void DrawDirectionalLightComponent(DirectionalLightComponent& directionLight);
    static void DrawTransformComponent(Entity* selectedEntity);
    static void DrawTransformComponent(TransformComponent& transform);
    void DrawMeshComponent(Entity* selectedEntity);
    void DrawMaterialComponent(Entity* selectedEntity);
    void DrawColliderComponent(Entity* selectedEntity);
    void DrawScriptComponent(Entity* selectedEntity);
    void DrawCameraComponent(Entity* selectedEntity);
    void DrawUiComponent(Entity* selectedEntity);
    void DrawAudioEmitterComponent(Entity* selectedEntity);
    void DrawAudioListenerComponent(Entity* selectedEntity);
    static void DrawAddComponent(Entity* selectedEntity);
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif