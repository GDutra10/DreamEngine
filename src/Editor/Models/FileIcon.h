#ifndef EDITOR_MODELS_FILE_ICON_H_
#define EDITOR_MODELS_FILE_ICON_H_

#include "../Vendors/imgui/imgui.h"
#include <glad/glad.h>

namespace DreamEngine::Editor::Models
{
struct FileIcon
{
    GLuint textureID;
    ImVec2 size;
};
}  // namespace DreamEngine::Editor::Models
#endif