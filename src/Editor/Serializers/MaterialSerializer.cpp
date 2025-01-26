#include "MaterialSerializer.h"

#include <sstream>

#include "Vec3Serializer.h"
#include "../../Core/Resources/GlobalResourceManager.h"
#include "../../Core/Loggers/LoggerSingleton.h"

using namespace DreamEngine::Editor::Serializers;
using namespace DreamEngine::Core::Loggers;

std::string MaterialSerializer::Serialize(const Material& material)
{
    std::ostringstream stringBuilder;

    stringBuilder << "Name: " << material.name << std::endl
         << "   ResourceId: " << material.resourceId << std::endl
         << "   Shader: " << material.shader->resourceId << std::endl
         << "   Ambient: " << Vec3Serializer::Serialize(material.ambient) << std::endl
         << "   Diffuse: " << Vec3Serializer::Serialize(material.diffuse) << std::endl
         << "   Specular: " << Vec3Serializer::Serialize(material.specular) << std::endl
         << "   Shininess: " << material.shininess << std::endl;

    return stringBuilder.str();
}

Material* MaterialSerializer::Deserialize(std::ifstream& stream)
{
    if (!stream.is_open())
    {
        LoggerSingleton::Instance().LogError("MaterialSerializer::Deserialize -> Could not open file");

        return nullptr;
    }

    std::string line;
    Material* material = new Material();

    while (std::getline(stream, line))
    {
        if (line.find("Name: ") != std::string::npos)
        {
            material->name = line.substr(6);
        }
        else if (line.find("ResourceId: ") != std::string::npos)
        {
            material->resourceId = line.substr(15);
        }
        else if (line.find("Shader: ") != std::string::npos)
        {
            material->shader = GlobalResourceManager::Instance().GetShader(line.substr(11));
        }
        else if (line.find("Ambient: ") != std::string::npos)
        {
            material->ambient = Vec3Serializer::Deserialize(line.substr(12));
        }
        else if (line.find("Diffuse: ") != std::string::npos)
        {
            material->diffuse = Vec3Serializer::Deserialize(line.substr(12));
        }
        else if (line.find("Specular: ") != std::string::npos)
        {
            material->specular = Vec3Serializer::Deserialize(line.substr(13));
        }
        else if (line.find("Shininess: ") != std::string::npos)
        {
            material->shininess = std::stof(line.substr(14));
        }
    }

    return material;
}
