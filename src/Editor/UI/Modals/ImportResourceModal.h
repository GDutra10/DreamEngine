#ifndef EDITOR_UI_MODALS_IMPORT_RESOURCE_MODAL_H_
#define EDITOR_UI_MODALS_IMPORT_RESOURCE_MODAL_H_
#include <string>
#include "BaseModal.h"
#include "FileDialogModal.h"
#include "GetNameModal.h"

namespace DreamEngine::Editor::UI::Modals
{
using namespace DreamEngine::Editor::Controllers;
class ImportResourceModal final : public BaseModal
{
   public:
    ImportResourceModal(std::string title);

   protected:
    GetNameModal m_getNamePopup;
    FileDialogModal m_fileDialog;
    void DrawContent() override;
    void CreateMeshesFromFile(const std::string& filePath);
};

}  // namespace DreamEngine::Editor::UI::Modals
#endif