#ifndef EDITOR_UI_WINDOWS_PROJECT_WINDOW_H_
#define EDITOR_UI_WINDOWS_PROJECT_WINDOW_H_

#include <filesystem>

#include "BaseWindow.h"
#include "../Modals/ImportResourceModal.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std::filesystem;
using namespace DreamEngine::Editor::UI::Modals;

class ProjectWindow : public BaseWindow
{
   public:
    ProjectWindow(const std::string& title);

   protected:
    void DrawContent() override;
   private:
    ImportResourceModal* m_importResourcePopup;
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif