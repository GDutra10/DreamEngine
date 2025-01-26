#ifndef CORE_RENDER_SHADER_H_
#define CORE_RENDER_SHADER_H_

#include <string>
#include "../../Vendors/glm/glm/glm.hpp"
#include "../Resources/Resource.h"

namespace DreamEngine::Core::Render
{
using namespace DreamEngine::Core::Resources;
class Shader : public Resource
{
   public:
    unsigned int id;
    char* fragmentShaderCode;
    char* vertexShaderCode;
    bool hasErrors;
    Shader() = default;
    virtual ~Shader() = default;
    // activate the shader
    virtual void Use() = 0;
    virtual void SetBool(const std::string& name, bool value) = 0;
    virtual void SetInt(const std::string& name, int value) = 0;
    virtual void SetFloat(const std::string& name, float value) = 0;
    virtual void SetVec2(const std::string& name, const glm::vec2& value) = 0;
    virtual void SetVec2(const std::string& name, float x, float y) = 0;
    virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;
    virtual void SetVec3(const std::string& name, float x, float y, float z) = 0;
    virtual void SetVec4(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetVec4(const std::string& name, float x, float y, float z, float w) = 0;
    virtual void SetMat2(const std::string& name, glm::mat2& mat) = 0;
    virtual void SetMat3(const std::string& name, glm::mat3& mat) = 0;
    virtual void SetMat4(const std::string& name, glm::mat4& mat) = 0;
};
}  // namespace DreamEngine::Core::Render
#endif