#ifndef EDITOR_MODELS_DATAS_DIRECTIONAL_LIGHT_DATA_H_
#define EDITOR_MODELS_DATAS_DIRECTIONAL_LIGHT_DATA_H_

#include "ColorData.h"
#include "Vector3Data.h"

namespace DreamEngine::Editor::Models::Datas
{
struct DirectionalLightData
{
    Vector3Data specular;
    ColorData color;
    float influence;
};

}  // namespace DreamEngine::Editor::Models
#endif