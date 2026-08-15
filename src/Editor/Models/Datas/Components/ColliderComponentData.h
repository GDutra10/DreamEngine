#ifndef EDITOR_MODELS_DATAS_COMPONENTS_COLLIDER_COMPONENT_DATA_H_
#define EDITOR_MODELS_DATAS_COMPONENTS_COLLIDER_COMPONENT_DATA_H_

#include "../Vector3Data.h"

namespace DreamEngine::Editor::Models::Datas::Components
{
struct ColliderComponentData
{
    bool has = false;
    bool enabled;
    bool isTrigger;
    Vector3Data center;
    Vector3Data size;
};

}  // namespace DreamEngine::Editor::Models::Datas::Components
#endif