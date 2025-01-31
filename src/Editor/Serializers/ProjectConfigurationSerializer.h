#ifndef EDITOR_SERIALIZERS_PROJECT_CONFIGURATION_SERIALIZER_H_
#define EDITOR_SERIALIZERS_PROJECT_CONFIGURATION_SERIALIZER_H_

#include <string>

#include <fstream>
#include "../Models/ProjectConfiguration.h"

namespace DreamEngine::Editor::Serializers
{
using namespace DreamEngine::Editor::Models;
class ProjectConfigurationSerializer
{
   public:
    static std::string Serialize(ProjectConfiguration& model);
    static ProjectConfiguration& Deserialize(std::ifstream& stream);
};

}  // namespace DreamEngine::Editor::Serializers
#endif