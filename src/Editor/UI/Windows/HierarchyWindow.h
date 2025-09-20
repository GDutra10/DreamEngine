#ifndef EDITOR_UI_WINDOWS_HIERARCHY_WINDOW_H_
#define EDITOR_UI_WINDOWS_HIERARCHY_WINDOW_H_

#include <filesystem>

#include "BaseWindow.h"
#include "../../Models/Datas/SceneData.h"
#include "../../Controllers/SceneController.h"

namespace DreamEngine::Editor::UI::Windows
{
    using namespace std::filesystem;
    using namespace DreamEngine::Editor::Models::Datas;
    using namespace DreamEngine::Editor::Controllers;

class HierarchyWindow final : public BaseWindow
{
   public:
    HierarchyWindow(const std::string& title);

protected:
    void DrawContent() override;

private:
    path m_selectedScenePath;
    void AddEntityTreeNode(Entity* entity);
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif