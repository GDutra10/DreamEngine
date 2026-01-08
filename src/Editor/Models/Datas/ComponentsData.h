#ifndef EDITOR_MODELS_DATAS_COMPONENTS_DATA_H_
#define EDITOR_MODELS_DATAS_COMPONENTS_DATA_H_

#include "Components/CameraComponentData.h"
#include "Components/ChildrenComponentData.h"
#include "Components/MaterialComponentData.h"
#include "Components/MeshComponentData.h"
#include "Components/ParentComponentData.h"
#include "Components/ScriptComponentData.h"
#include "Components/UiComponentData.h"

namespace DreamEngine::Editor::Models::Datas
{
using namespace DreamEngine::Editor::Models::Datas::Components;

struct ComponentsData
{
    ChildrenComponentData children;
    MaterialComponentData material;
    MeshComponentData mesh;
    ParentComponentData parent;
    ScriptComponentData script;
    CameraComponentData camera;
    UiComponentData ui;
};

}  // namespace DreamEngine::Editor::Models::Datas
#endif