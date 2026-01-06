#ifndef EDITOR_MODELS_DATAS_SCENE_DATA_H_
#define EDITOR_MODELS_DATAS_SCENE_DATA_H_

#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include "EntityConfigData.h"
#include "ColorData.h"
#include "GlobalLightData.h"
#include <Application.h>

namespace DreamEngine::Editor::Models::Datas
{
struct SceneData
{
    filesystem::path path;
    ColorData backgroundColor;
    GlobalLightData globalLight;
    std::string mainCameraEntityIdentifier;
    std::vector<EntityConfigData> entities;
};

}  // namespace DreamEngine::Editor::Models
#endif