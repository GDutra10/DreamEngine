#ifndef EDITOR_UI_WINDOWS_TEXT_EDITOR_WINDOW_H_
#define EDITOR_UI_WINDOWS_TEXT_EDITOR_WINDOW_H_

#include <functional>
#include <filesystem>

#include "BaseWindow.h"
#include "../../Vendors/ImGuiColorTextEdit/TextEditor.h"
#include "../../Controllers/ResourceController.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std;
using namespace std::filesystem;
using namespace DreamEngine::Editor::Controllers;

class TextEditorWindow : public BaseWindow
{
   public:
    TextEditorWindow(const string& title, EditorContext& editorContext, ResourceController& resourceController);

   protected:
    void DrawContent() override;

   private:
    ResourceController& m_resourceController;
    TextEditor m_textEditor;
    path m_filePath;
    bool m_savedFile;
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif