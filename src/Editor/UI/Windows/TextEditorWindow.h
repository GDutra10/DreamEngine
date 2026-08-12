#ifndef EDITOR_UI_WINDOWS_TEXT_EDITOR_WINDOW_H_
#define EDITOR_UI_WINDOWS_TEXT_EDITOR_WINDOW_H_

#include <functional>
#include <filesystem>

#include "BaseWindow.h"
#include "../../Vendors/ImGuiColorTextEdit/TextEditor.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std;
using namespace std::filesystem;

class TextEditorWindow : public BaseWindow
{
   public:
    TextEditorWindow(const string& title);

   protected:
    void DrawContent() override;

   private:
    TextEditor m_textEditor;
    path m_filePath;
    bool m_savedFile;
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif