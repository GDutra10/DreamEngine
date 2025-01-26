#ifndef EDITOR_UI_MODALS_GET_NAME_MODAL_H_
#define EDITOR_UI_MODALS_GET_NAME_MODAL_H_

#include <string>
#include <functional>
#include "BaseModal.h"
#include "../../Controllers/ResourceController.h"

namespace DreamEngine::Editor::UI::Modals
{
using namespace DreamEngine::Editor::Controllers;
class GetNameModal final : public BaseModal
{
   public:
    explicit GetNameModal(std::string title);
    void Open(std::string title, const std::function<Result(std::string name)>& callbackOk);

   protected:
    void DrawContent() override;
    char m_name[128] = "";
    std::string m_errorMessage;
    std::function<Result(std::string name)> m_callbackOk;
    ResourceController m_resourceController;
};

}  // namespace DreamEngine::Editor::UI::Modals
#endif