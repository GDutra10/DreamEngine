#ifndef CORE_RENDER_MATERIAL_H_
#define CORE_RENDER_MATERIAL_H_

#include "Shader.h"
#include "../Resources/Resource.h"

namespace DreamEngine::Core::Render
{
using namespace DreamEngine::Core::Resources;

class Material : public Resource
{
public:
    Shader* shader = nullptr;
    glm::vec3 ambient = { 0, 0, 0 };
    glm::vec3 diffuse = {0, 0, 0};
    glm::vec3 specular = {0, 0, 0};
    float shininess = 0;
};
}  // namespace DreamEngine::Core::Render
#endif