#ifndef EDITOR_MODELS_EDITOR_CONFIGURATION_H_
#define EDITOR_MODELS_EDITOR_CONFIGURATION_H_

#include <map>
#include <string>
#include "../../Vendors/glm/glm/glm.hpp"

namespace DreamEngine::Editor::Models
{
struct EditorConfiguration
{
    glm::vec2 windowSize;
    static EditorConfiguration LoadEditorConfiguration();
};

}  // namespace DreamEngine::Editor::Models
#endif