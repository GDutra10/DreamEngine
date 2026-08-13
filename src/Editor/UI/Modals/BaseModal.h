#ifndef EDITOR_UI_MODALS_BASE_MODAL_H_
#define EDITOR_UI_MODALS_BASE_MODAL_H_
#include <string>
#include "../../EditorContext.h"

namespace DreamEngine::Editor::UI::Modals
{
using namespace DreamEngine::Editor;
class BaseModal
{
   public:
    
    BaseModal(std::string title, EditorContext& editorContext);
    virtual ~BaseModal() = default;
    void Open();
    void Close();
    void Draw();
    float minHeight = 0.0f;
    float maxHeight = FLT_MAX;
    float minWidth = 0.0f;
    float maxWidth = FLT_MAX;

   protected:
    EditorContext& m_editorContext;
    bool m_isOpen = false;
    std::string m_title;
    virtual void DrawContent() = 0;
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif