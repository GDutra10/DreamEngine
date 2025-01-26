#ifndef CORE_RENDER_OPEN_GL_OPEN_GL_SHADER_H_
#define CORE_RENDER_OPEN_GL_OPEN_GL_SHADER_H_

#include <string>
#include <glad/glad.h>
#include "../Shader.h"
#include "glm/gtc/type_ptr.hpp"

namespace DreamEngine::Core::Render::OpenGL
{
class OpenGLShader : public Shader
{
   public:
    explicit OpenGLShader(int id) { this->id = id; }

    // activate the shader
    void Use() override { glUseProgram(id); }

    // utility uniform functions
    void SetBool(const std::string& name, bool value) override
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }

    void SetInt(const std::string& name, int value) override
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void SetFloat(const std::string& name, float value) override
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void SetVec2(const std::string& name, const glm::vec2& value) override
    {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value.x);
    }
    void SetVec2(const std::string& name, float x, float y) override
    {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }

    void SetVec3(const std::string& name, const glm::vec3& value) override
    {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value.x);
    }

    void SetVec3(const std::string& name, float x, float y, float z) override
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }

    void SetVec4(const std::string& name, const glm::vec4& value) override
    {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }

    void SetVec4(const std::string& name, float x, float y, float z,
                 float w) override
    {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }

    void SetMat2(const std::string& name, glm::mat2& mat) override
    {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void SetMat3(const std::string& name, glm::mat3& mat) override
    {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void SetMat4(const std::string& name, glm::mat4& mat) override
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
};

}  // namespace DreamEngine::Core::Render::OpenGL
#endif