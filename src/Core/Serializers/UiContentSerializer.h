#ifndef CORE_SERIALIZERS_UI_CONTENT_SERIALIZER_H_
#define CORE_SERIALIZERS_UI_CONTENT_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../Core/UI/UiContent.h"

namespace DreamEngine::Core::Serializers
{
using namespace DreamEngine::Core::UI;
class CORE_API UiContentSerializer
{
   public:
    static std::string Serialize(const UiContent& model);
    static UiContent* Deserialize(const std::ifstream& stream);
};

}  // namespace DreamEngine::Core::Serializers
#endif