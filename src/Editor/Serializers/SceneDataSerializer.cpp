#include "SceneDataSerializer.h"
#include <nlohmann/json.hpp>
#include "../Models/SceneConfig.h"
#include "../Models/EntityConfig.h"
#include "../../Core/GameSystem/Camera.h"
#include "../../Core/GameSystem/GlobalLight.h"
#include "../../Core/ECS/Components/TransformComponent.h"
#include "../../Core/ECS/Components/MeshComponent.h"
#include "../Controllers/CameraEditorController.h"

using namespace DreamEngine::Editor::Serializers;
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Core::GameSystem;
using namespace DreamEngine::Core::ECS::Components;
using json = nlohmann::json;

std::string SceneDataSerializer::Serialize(SceneData& model)
{
    json j;
    /*j["config"] = model.config;
    j["entities"] = model.entities;*/

    return j.dump();
}

SceneData& SceneDataSerializer::Deserialize(std::ifstream& stream)
{
    json j;
    stream >> j;

    SceneData* model = new SceneData();
    /*model->config = j["config"].get<SceneConfig>();
    model->entities = j["entities"].get<std::vector<EntityConfig>>();*/

    return *model;
}