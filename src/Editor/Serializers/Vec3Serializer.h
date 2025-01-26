#ifndef EDITOR_SERIALIZERS_VEC_3_SERIALIZER_H_
#define EDITOR_SERIALIZERS_VEC_3_SERIALIZER_H_

#include <string>
#include "../../Vendors/glm/glm/glm.hpp"

namespace DreamEngine::Editor::Serializers
{
class Vec3Serializer
{
   public:
    static std::string Serialize(const glm::vec3 vector3);
    static glm::vec3 Deserialize(const std::string& serializedVec3);
};

}  // namespace DreamEngine::Editor::Serializers
#endif