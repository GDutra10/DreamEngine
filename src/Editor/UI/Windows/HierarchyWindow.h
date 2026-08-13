#ifndef EDITOR_UI_WINDOWS_HIERARCHY_WINDOW_H_
#define EDITOR_UI_WINDOWS_HIERARCHY_WINDOW_H_

#include <filesystem>

#include "BaseWindow.h"
#include "../../Models/Datas/SceneData.h"
#include "../../Controllers/SceneController.h"
#include "../../Controllers/EntityController.h"

namespace DreamEngine::Editor::UI::Windows
{
    using namespace std::filesystem;
    using namespace DreamEngine::Editor::Models::Datas;
    using namespace DreamEngine::Editor::Controllers;

class HierarchyWindow final : public BaseWindow
{
   public:
    HierarchyWindow(const std::string& title, EditorContext& editorContext, SceneController& sceneController, EntityController& entityController);

protected:
    void DrawContent() override;

private:
    SceneController& m_sceneController;
    EntityController& m_entityController;
    path m_selectedScenePath;
    void AddEntityTreeNode(Entity* entity);
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif