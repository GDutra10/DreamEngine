#ifndef EDITOR_UI_WINDOWS_PROJECT_WINDOW_H_
#define EDITOR_UI_WINDOWS_PROJECT_WINDOW_H_

#include <filesystem>

#include "BaseWindow.h"
#include "../Modals/ImportResourceModal.h"
#include "../../Controllers/ResourceController.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std::filesystem;
using namespace DreamEngine::Editor::UI::Modals;
using namespace DreamEngine::Editor::Controllers;

class ProjectWindow : public BaseWindow
{
   public:
    ProjectWindow(const std::string& title, EditorContext& editorContext, ResourceController& resourceController);

   protected:
    void DrawContent() override;
   private:
    ImportResourceModal* m_importResourcePopup;
    FileDialogConfig m_fileDialogConfig;
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif