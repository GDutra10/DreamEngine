
#ifndef EDITOR_MODELS_SCENE_CONFIG_H_
#define EDITOR_MODELS_SCENE_CONFIG_H_

#include <map>
#include <string>
#include "../../Vendors/glm/glm/glm.hpp"
#include "../../Core/GameSystem/Camera.h"
#include "../../Core/GameSystem/GlobalLight.h"

namespace DreamEngine::Editor::Models
{
using namespace DreamEngine::Core::GameSystem;
struct SceneConfig
{
    glm::vec3 backgroundColor;
    Camera camera;
    GlobalLight globalLight;
};
}  // namespace DreamEngine::Editor::Models
#endif