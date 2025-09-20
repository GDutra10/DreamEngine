#ifndef EDITOR_SERIALIZERS_MODEL_SERIALIZER_H_
#define EDITOR_SERIALIZERS_MODEL_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../Models/Model.h"

namespace DreamEngine::Editor::Serializers
{
using namespace DreamEngine::Editor::Models;
class ModelSerializer
{
   public:
    static std::string Serialize(const Model& model);
    static Model& Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Editor::Serializers
#endif