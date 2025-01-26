#ifndef CORE_COLOR_H_
#define CORE_COLOR_H_
#include <glm/glm.hpp>

namespace DreamEngine::Core
{
struct Color
{
    float red = 0;
    float green = 0;
    float blue = 0;
    float alpha = 0;

    glm::vec3 ToVec3() { return {red, green, blue}; }
    glm::vec4 ToVec4() { return {red, green, blue, alpha}; }
};
}  // namespace DreamEngine::Core
#endif