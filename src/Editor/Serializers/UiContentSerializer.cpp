#include "UiContentSerializer.h"
#include <fstream>
#include <sstream>

using namespace DreamEngine::Editor::Serializers;

std::string UiContentSerializer::Serialize(const UiContent& model)
{
    // TODO: generate json

    return model.text;
}

UiContent* UiContentSerializer::Deserialize(const std::ifstream& stream)
{
    UiContent* uiContent = new UiContent();
    std::ostringstream ss;    
    ss << stream.rdbuf();
    uiContent->text = ss.str();
    ;

    return uiContent;
}