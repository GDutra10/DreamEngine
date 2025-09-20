#ifndef EDITOR_MODELS_DATAS_ENTITY_CONFIG_DATA_H_
#define EDITOR_MODELS_DATAS_ENTITY_CONFIG_DATA_H_

#include <string>

#include "TransformData.h"
#include "ComponentsData.h"
#include "../../Core/ECS/Components/MeshComponent.h"

namespace DreamEngine::Editor::Models::Datas
{
    using namespace std;
    using namespace DreamEngine::Core::ECS::Components;

struct EntityConfigData
{
    string tag;
    string name;
    string identifier;
    bool isActive;
    TransformData transform;
    ComponentsData components;
};
}  // namespace DreamEngine::Editor::Models
#endif