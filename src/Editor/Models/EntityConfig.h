#ifndef EDITOR_MODELS_ENTITY_CONFIG_H_
#define EDITOR_MODELS_ENTITY_CONFIG_H_

#include <map>
#include <string>
#include <vector>
#include "SceneConfig.h"
#include "../../Core/ECS/Components/TransformComponent.h"
#include "../../Core/ECS/Components/MeshComponent.h"

namespace DreamEngine::Editor::Models
{
    using namespace std;
    using namespace DreamEngine::Core::ECS::Components;

struct EntityConfig
{
    size_t id;
    string tag;
    string name;
    vector<std::string> components;
    TransformComponent transform;
    MeshComponent mesh;
};
}  // namespace DreamEngine::Editor::Models
#endif