#ifndef CORE_GAME_SYSTEM_CAMERA_H_
#define CORE_GAME_SYSTEM_CAMERA_H_

#include "../../Vendors/glm/glm/glm.hpp"
#include "../../Vendors/glm/glm/ext/matrix_clip_space.hpp"
#include "../../Vendors/glm/glm/ext/matrix_transform.hpp"

namespace DreamEngine::Core::GameSystem
{
struct Camera
{
    glm::vec3 position = {0, 0, 0};
    glm::vec3 front = {0, 0, 0};
    glm::vec3 up = {0, 1, 0};
    glm::vec3 right = {0, 0, 0};
    glm::vec3 worldUp = {0, 1, 0};

    float fovDegree = 45.0f;
    float near = 0.1f;
    float far = 100.0f;

    [[nodiscard]] glm::mat4 GetView() const
    {
        return glm::lookAt(position, position + front, up);
    }

    /*[[nodiscard]] glm::mat4 GetProjection(const float aspect) const
    {
        return glm::perspective(
            glm::radians(fovDegree), 
            aspect,
            near,
            far);
    }*/

    [[nodiscard]] glm::mat4 GetProjection() const
    {
        return m_projectionMatrix;
    }

    void SetPerspectiveProjectionMatrix(const float radFov, const float width, const float height, const float nearP, const float farP)
    {
        m_projectionMatrix = glm::perspective(radFov, width / height, nearP, farP);
    }

   private:
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
};
}  // namespace DreamEngine::Core::GameSystem
#endif