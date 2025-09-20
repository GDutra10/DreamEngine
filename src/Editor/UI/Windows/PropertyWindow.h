#ifndef EDITOR_UI_WINDOWS_PROPERTY_WINDOW_H_
#define EDITOR_UI_WINDOWS_PROPERTY_WINDOW_H_

#include "BaseWindow.h"
#include "../../Singletons/EditorSingleton.h"
#include "../Views/MaterialView.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::UI::Views;

class PropertyWindow : public BaseWindow
{
   public:
    PropertyWindow(const std::string& title);

   protected:
    void DrawContent() override;

    private:
    MaterialView m_materialView;
    static void DrawSceneData();
    // Draw components
    static void DrawDirectionalLightComponent(Entity* selectedEntity);
    static void DrawDirectionalLightComponent(DirectionalLightComponent& directionLight);
    static void DrawTransformComponent(Entity* selectedEntity);
    static void DrawTransformComponent(TransformComponent& transform);
    void DrawMeshComponent(Entity* selectedEntity);
    void DrawMaterialComponent(Entity* selectedEntity);
    void DrawScriptComponent(Entity* selectedEntity);
    static void DrawAddComponent(Entity* selectedEntity);
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif