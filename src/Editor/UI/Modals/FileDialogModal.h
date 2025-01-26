#ifndef EDITOR_UI_MODALS_FILE_DIALOG_MODAL_H_
#define EDITOR_UI_MODALS_FILE_DIALOG_MODAL_H_

#include <functional>

#include "BaseModal.h"

namespace DreamEngine::Editor::UI::Modals
{
class FileDialogModal final : public BaseModal
{
   public:
    std::string selectedFile;
    std::string currentPath;
    FileDialogModal(const std::string& title);
    void Open(const std::function<void(std::string)>& callbackOk);

   protected:
    std::function<void(std::string)> m_callbackOk;
    void DrawContent() override;
};

}  // namespace DreamEngine::Editor::UI::Modals
#endif