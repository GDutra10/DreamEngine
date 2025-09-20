#ifndef EDITOR_MODELS_DATAS_TRANSFORM_DATA_H_
#define EDITOR_MODELS_DATAS_TRANSFORM_DATA_H_

#include "Vector3Data.h"

namespace DreamEngine::Editor::Models::Datas
{
struct TransformData
{
    Vector3Data position;
    Vector3Data rotation;
    Vector3Data scale;
};

}  // namespace DreamEngine::Editor::Models
#endif