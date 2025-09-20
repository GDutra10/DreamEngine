#ifndef EDITOR_MODELS_DATAS_GLOBAL_LIGHT_DATA_H_
#define EDITOR_MODELS_DATAS_GLOBAL_LIGHT_DATA_H_

#include "TransformData.h"
#include "DirectionalLightData.h"

namespace DreamEngine::Editor::Models::Datas
{
struct GlobalLightData
{
    TransformData transform;
    DirectionalLightData directionalLight;
};

}  // namespace DreamEngine::Editor::Models
#endif