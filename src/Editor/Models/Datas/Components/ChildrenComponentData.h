#ifndef EDITOR_MODELS_DATAS_COMPONENTS_CHILDREN_COMPONENT_DATA_H_
#define EDITOR_MODELS_DATAS_COMPONENTS_CHILDREN_COMPONENT_DATA_H_

#include <string>
#include <vector>

namespace DreamEngine::Editor::Models::Datas::Components
{
struct ChildrenComponentData
{
    std::vector<std::string> childIdentifiers;
};

}  // namespace DreamEngine::Editor::Models::Datas::Components
#endif