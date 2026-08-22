#include "TextEditorWindow.h"

#include <iostream>
#include <fstream>
#include <string>

#include "../../EditorDefine.h"
#include "../../Helpers/FileHelper.h"
#include "../../Core/Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor::UI::Windows;
using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::Controllers;

TextEditorWindow::TextEditorWindow(const string& title, EditorContext& editorContext, ResourceController& resourceController) 
    : BaseWindow(title, editorContext)
    , m_resourceController(resourceController) {}

void TextEditorWindow::DrawContent() 
{
    auto fileExtension = m_filePath.extension();

    if (m_filePath != m_editorContext.GetFileTextEditorPath())
    {
        m_filePath = m_editorContext.GetFileTextEditorPath();
        
        const auto& languageDef = fileExtension == EDITOR_DEFAULT_UI_FILE_EXTENSION       
            ? TextEditor::LanguageDefinition::HTML()
            : fileExtension == EDITOR_DEFAULT_CSHARP_FILE_EXTENSION 
                ? TextEditor::LanguageDefinition::CSharp() 
                : TextEditor::LanguageDefinition();

        m_textEditor.SetLanguageDefinition(languageDef);

        std::ifstream file(m_filePath);

        if (file.is_open())
        {
            std::stringstream buffer;
            
            buffer << file.rdbuf();

            m_textEditor.SetText(buffer.str());

            file.close();
        }
    }

    if (this->IsFocused() && Input::IsKeyDown(Key::LEFT_CONTROL) && Input::IsKeyDown(Key::S) && !m_savedFile)
    {
        LoggerSingleton::Instance().LogDebug("Save File");

        std::ofstream file(m_filePath);
        
        if (file.is_open())
        {
            file << m_textEditor.GetText().c_str();
            file.close();
        }

        if (fileExtension == EDITOR_DEFAULT_UI_FILE_EXTENSION)
            m_resourceController.ReloadUiComponent(m_filePath);

        m_savedFile = true;
    }

    if (m_textEditor.IsTextChanged())
        m_savedFile = false;

    // render fields
    ImGui::Text(m_filePath.string().c_str());
    m_textEditor.Render(m_title.c_str(), ImGui::GetContentRegionAvail());
}
