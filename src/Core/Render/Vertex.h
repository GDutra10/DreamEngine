#ifndef CORE_RENDER_VERTEX_H_
#define CORE_RENDER_VERTEX_H_

#define MAX_BONE_INFLUENCE 4

#include "../../Vendors/glm/glm/glm.hpp"

#include "CoreExport.h"

namespace DreamEngine::Core::Render
{
class CORE_API Vertex
{
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;    
    glm::vec3 bitangent;
    //bone indexes which will influence this vertex
    int boneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float weights[MAX_BONE_INFLUENCE];
};
}  // namespace DreamEngine::Core::Render
#endif