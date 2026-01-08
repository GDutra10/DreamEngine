#ifndef CORE_UI_RML_RML_UI_INSTANCE_H_
#define CORE_UI_RML_RML_UI_INSTANCE_H_

#include <string>
#include <unordered_set>

#include "RmlClickBind.h"
#include "RmlUi/Core/DataModelHandle.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/Variant.h"
#include "UI/UiInstance.h"

namespace DreamEngine::Core::UI::RmlUI
{
class RmlUiInstance : public UiInstance
{
public:
    Rml::ElementDocument* document = nullptr;

    std::string rmlText;

    Rml::DataModelConstructor dataModelConstructor;
    Rml::DataModelHandle dataModelHandle;

    std::unordered_map<std::string, Rml::Variant> vars;
    std::unordered_set<std::string> bound;
    std::vector<RmlClickBinding> clickBindings;
};
}  // namespace DreamEngine::Core::UI::RmlUI
#endif