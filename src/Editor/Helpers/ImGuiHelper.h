#ifndef EDITOR_HELPERS_IM_GUI_HELPER_H_
#define EDITOR_HELPERS_IM_GUI_HELPER_H_

#include <functional>
#include <string>
#include <filesystem>
#include "../Vendors/imgui/imgui.h"
#include "../Models/FileDialogConfig.h"

namespace DreamEngine::Editor::Helpers
{
using namespace DreamEngine::Editor::Models;
using namespace std::filesystem;
class ImGuiHelper
{
   public:
    const static ImVec4& GetImVec4Red() { return m_sImVec4Red; }
    const static ImVec4& GetImVec4Green() { return m_sImVec4Green; }
    const static ImVec4& GetImVec4Blue() { return m_sImVec4Blue; }
    static void PrepareRow(const char* label);
    static void NextRow(const char* id, const char* label, std::string& value, bool editable = true);
    static bool BeginTable(const char* id, unsigned int columns);
    static void DrawDirectoryTree(const std::filesystem::path& directoryPath, std::string& selectedPath, const std::function<void(std::filesystem::path)>& selectedPathCallback,
                                  Models::FileDialogConfig config = {});

   private:
    static ImVec4 m_sImVec4Red; 
    static ImVec4 m_sImVec4Green;
    static ImVec4 m_sImVec4Blue;
};

}  // namespace DreamEngine::Editor::Helpers
#endif