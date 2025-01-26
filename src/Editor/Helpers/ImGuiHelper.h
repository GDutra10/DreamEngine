#ifndef EDITOR_HELPERS_IM_GUI_HELPER_H_
#define EDITOR_HELPERS_IM_GUI_HELPER_H_

#include <string>
#include "../Vendors/imgui/imgui.h"

namespace DreamEngine::Editor::Helpers
{
class ImGuiHelper
{
   public:
    const static ImVec4& GetImVec4Red() { return m_sImVec4Red; }
    const static ImVec4& GetImVec4Green() { return m_sImVec4Green; }
    const static ImVec4& GetImVec4Blue() { return m_sImVec4Blue; }
    static void PrepareRow(const char* label);
    static void NextRow(const char* id, const char* label, std::string& value, bool editable = true);
    static bool BeginTable(const char* id, unsigned int columns);

   private:
    static ImVec4 m_sImVec4Red; 
    static ImVec4 m_sImVec4Green;
    static ImVec4 m_sImVec4Blue;
};

}  // namespace DreamEngine::Editor::Helpers
#endif