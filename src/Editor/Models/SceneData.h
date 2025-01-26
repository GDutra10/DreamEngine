#ifndef EDITOR_MODELS_SCENE_DATA_H_
#define EDITOR_MODELS_SCENE_DATA_H_

#include <map>
#include <string>
#include <vector>
#include "SceneConfig.h"
#include "EntityConfig.h"

namespace DreamEngine::Editor::Models
{
struct SceneData
{
    SceneConfig config;
    std::vector<EntityConfig> entities;
};

}  // namespace DreamEngine::Editor::Models
#endif