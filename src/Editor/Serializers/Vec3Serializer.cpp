#include <sstream>
#include "Vec3Serializer.h"

using namespace DreamEngine::Editor::Serializers;

std::string Vec3Serializer::Serialize(const glm::vec3 vector3)
{
    std::ostringstream stringBuilder;

    stringBuilder << "(" << vector3.x << ", " << vector3.y << ", " << vector3.z << ")";

    return stringBuilder.str();
}

glm::vec3 Vec3Serializer::Deserialize(const std::string& serializedVec3)
{
    glm::vec3 vector3;
    std::string values = serializedVec3.substr(1, serializedVec3.size() - 2);
    std::istringstream stream(values);
    std::string value;

    std::getline(stream, value, ',');
    vector3.x = std::stof(value);

    std::getline(stream, value, ',');
    vector3.y = std::stof(value);

    std::getline(stream, value, ',');
    vector3.z = std::stof(value);

    return vector3;
}