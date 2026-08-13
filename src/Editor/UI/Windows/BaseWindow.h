#ifndef EDITOR_UI_WINDOWS_BASE_WINDOW_H_
#define EDITOR_UI_WINDOWS_BASE_WINDOW_H_
#include <string>
#include "../../EditorContext.h"

namespace DreamEngine::Editor::UI::Windows
{
class BaseWindow
{
public:
    bool isOpen = false;
    BaseWindow(const std::string& title, EditorContext& editorContext);
    virtual ~BaseWindow() = default;
    void Draw();
    [[nodiscard]] bool IsFocused() const;
   protected:
    std::string m_title;
    EditorContext& m_editorContext;
    bool m_isFocused = false;
    virtual void DrawContent() = 0;

};

}  // namespace DreamEngine::Editor::UI::Windows
#endif