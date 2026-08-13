#ifndef EDITOR_UI_MODALS_OPEN_PROJECT_MODAL_H_
#define EDITOR_UI_MODALS_OPEN_PROJECT_MODAL_H_
#include <filesystem>
#include <string>
#include "BaseModal.h"
#include "FileDialogModal.h"
#include "../../Controllers/ProjectController.h"

namespace DreamEngine::Editor::UI::Modals
{
using namespace DreamEngine::Editor::Controllers;
class OpenProjectModal final : public BaseModal
{
   public:
    OpenProjectModal(std::string title, EditorContext& editorContext, ProjectController& projectController);

   protected:
    ProjectController& m_projectController;
    char* m_newProjectName = new char[100];
    std::filesystem::path m_selectedPath;
    FileDialogModal m_fileDialog;
    void DrawContent() override;
};

}  // namespace DreamEngine::Editor::UI::Modals
#endif