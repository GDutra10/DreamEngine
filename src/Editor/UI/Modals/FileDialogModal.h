#ifndef EDITOR_UI_MODALS_FILE_DIALOG_MODAL_H_
#define EDITOR_UI_MODALS_FILE_DIALOG_MODAL_H_

#include <functional>

#include "BaseModal.h"
#include "../../Models/FileDialogConfig.h"

namespace DreamEngine::Editor::UI::Modals
{
using namespace DreamEngine::Editor::Models;
class FileDialogModal final : public BaseModal
{
   public:
    std::string selectedFile;
    std::string currentPath;
    FileDialogModal(const std::string& title);
    void Open(const std::function<bool(std::string)>& callbackOk, const FileDialogConfig& config = {});

   protected:
    std::function<bool(std::string)> m_callbackOk;
    FileDialogConfig m_config;
    void DrawContent() override;
};
}  // namespace DreamEngine::Editor::UI::Modals
#endif