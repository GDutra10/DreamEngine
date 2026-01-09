#ifndef EDITOR_MODELS_DATAS_COMPONENTS_UI_COMPONENT_DATA_H_
#define EDITOR_MODELS_DATAS_COMPONENTS_UI_COMPONENT_DATA_H_

#include <string>

namespace DreamEngine::Editor::Models::Datas::Components
{
struct UiComponentData
{
    bool has = false;
    int zOrder = 0;
    std::string resourceId = "";
};

}  // namespace DreamEngine::Editor::Models::Datas::Components
#endif