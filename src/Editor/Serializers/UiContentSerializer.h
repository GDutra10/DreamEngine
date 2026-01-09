#ifndef EDITOR_SERIALIZERS_UI_CONTENT_SERIALIZER_H_
#define EDITOR_SERIALIZERS_UI_CONTENT_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../Models/Model.h"
#include "../Core/UI/UiContent.h"

namespace DreamEngine::Editor::Serializers
{
using namespace DreamEngine::Core::UI;
class UiContentSerializer
{
   public:
    static std::string Serialize(const UiContent& model);
    static UiContent* Deserialize(const std::ifstream& stream);
};

}  // namespace DreamEngine::Editor::Serializers
#endif