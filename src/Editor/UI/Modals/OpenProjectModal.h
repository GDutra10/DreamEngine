#ifndef EDITOR_UI_MODALS_OPEN_PROJECT_MODAL_H_
#define EDITOR_UI_MODALS_OPEN_PROJECT_MODAL_H_
#include <filesystem>
#include <string>
#include "BaseModal.h"
#include "FileDialogModal.h"

namespace DreamEngine::Editor::UI::Modals
{
class OpenProjectModal final : public BaseModal
{
   public:
    OpenProjectModal(std::string title);

   protected:
    char* m_newProjectName = new char[100];
    std::filesystem::path m_selectedPath;
    FileDialogModal m_fileDialog;
    void DrawContent() override;
};

}  // namespace DreamEngine::Editor::UI::Modals
#endif