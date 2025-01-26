#ifndef EDITOR_UI_WINDOWS_BASE_WINDOW_H_
#define EDITOR_UI_WINDOWS_BASE_WINDOW_H_
#include <string>

namespace DreamEngine::Editor::UI::Windows
{
class BaseWindow
{
public:
    bool isOpen = false;
    BaseWindow(const std::string& title);
    virtual ~BaseWindow() = default;
    void Draw();
    [[nodiscard]] bool IsFocused() const;
   protected:
    std::string m_title;
    bool m_isFocused = false;
    virtual void DrawContent() = 0;

};

}  // namespace DreamEngine::Editor::UI::Windows
#endif