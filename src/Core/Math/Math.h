#ifndef CORE_MATH_MATH_H
#define CORE_MATH_MATH_H

#include <array>
#include <cmath>

#include "glm/gtc/epsilon.hpp"
#include "glm/gtc/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/quaternion.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace DreamEngine::Core::Math
{
    /*inline float degreesToRadians(float degrees)
    {
        return degrees * (M_PI / 180.0f);
    }

    inline Matrix4 perspective(const float fov, const float aspect, const float near, const float far)
    {
        Matrix4 result;

        float tanHalfFov = tan(fov / 2.0f);

        result.elements[0][0] = 1.0f / (aspect * tanHalfFov);
        result.elements[1][1] = -1.0f / tanHalfFov;
        result.elements[2][2] = -(far + near) / (far - near);
        result.elements[2][3] = -1.0f;
        result.elements[3][2] = -(2.0f * far * near) / (far - near);
        result.elements[3][3] = 0.0f;

        return result;
    }

    inline Matrix4 orthographic(const float left, 
                                const float right, 
                                const float bottom,
                                const float top, 
                                const float near,
                                const float far)
    {
        Matrix4 result;

        result.elements[0][0] = 2.0f / (right - left);
        result.elements[1][1] = 2.0f / (top - bottom);
        result.elements[2][2] = -2.0f / (far - near);
        result.elements[3][0] = -(right + left) / (right - left);
        result.elements[3][1] = -(top + bottom) / (top - bottom);
        result.elements[3][2] = -(far + near) / (far - near);

        return result;
    }

    inline Matrix4 lookAt(const Vector3& cameraPos, const Vector3& target, const Vector3& up)
    {
        Matrix4 result;

        const Vector3 f = (target - cameraPos).Normalize();
        const Vector3 s = f.Cross(up).Normalize();
        const Vector3 u = s.Cross(f);

        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;
        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;
        result[0][2] = -f.x;
        result[1][2] = -f.y;
        result[2][2] = -f.z;
        result[3][0] = -s.Dot(cameraPos);
        result[3][1] = -u.Dot(cameraPos);
        result[3][2] = f.Dot(cameraPos);

        return result;
    }*/

    inline CORE_API glm::vec3 scaleVector(const glm::vec3& v, float desiredLength)
	{
		float mag = glm::length(v);
		if (glm::epsilonEqual(mag, 0.0f, glm::epsilon<float>()))
			return glm::vec3(0.0f);

		return v * desiredLength / mag;
	}

    inline CORE_API glm::mat4 composeTransformation(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale)
    {
        glm::mat4 result(1.f);

        result = glm::translate(result, position);
        result = glm::rotate(result, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        result = glm::rotate(result, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        result = glm::rotate(result, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        result = glm::scale(result, scale);

        return result;
    }

    inline CORE_API bool decompose(const glm::mat4& transform, glm::vec3& translation,
                            glm::quat& rotation, glm::vec3& scale)
    {
        using namespace glm;
        using T = float;

        mat4 LocalMatrix(transform);

        if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
            return false;

        // Next take care of translation (easy).
        translation = vec3(LocalMatrix[3]);
        LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

        vec3 Row[3];

        // Now get scale and shear.
        for (length_t i = 0; i < 3; ++i)
            for (length_t j = 0; j < 3; ++j)
                Row[i][j] = LocalMatrix[i][j];

        // Compute X scale factor and normalize first row.
        scale.x = length(Row[0]);
        Row[0] = scaleVector(Row[0], static_cast<T>(1));

        // Now, compute Y scale and normalize 2nd row.
        scale.y = length(Row[1]);
        Row[1] = scaleVector(Row[1], static_cast<T>(1));
        //Skew.z /= Scale.y;

        // Next, get Z scale and normalize 3rd row.
        scale.z = length(Row[2]);
        Row[2] = scaleVector(Row[2], static_cast<T>(1));
        //Skew.y /= Scale.z;
        //Skew.x /= Scale.z;


        // Rotation as quaternion
        int i, j, k = 0;
        T root, trace = Row[0].x + Row[1].y + Row[2].z;
        if (trace > static_cast<T>(0))
        {
            root = sqrt(trace + static_cast<T>(1));
            rotation.w = static_cast<T>(0.5) * root;
            root = static_cast<T>(0.5) / root;
            rotation.x = root * (Row[1].z - Row[2].y);
            rotation.y = root * (Row[2].x - Row[0].z);
            rotation.z = root * (Row[0].y - Row[1].x);
        }  // End if > 0
        else
        {
            static int Next[3] = {1, 2, 0};
            i = 0;
            if (Row[1].y > Row[0].x)
                i = 1;
            if (Row[2].z > Row[i][i])
                i = 2;
            j = Next[i];
            k = Next[j];

            root =
                sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<T>(1.0));

            rotation[i] = static_cast<T>(0.5) * root;
            root = static_cast<T>(0.5) / root;
            rotation[j] = root * (Row[i][j] + Row[j][i]);
            rotation[k] = root * (Row[i][k] + Row[k][i]);
            rotation.w = root * (Row[j][k] - Row[k][j]);
        }  // End if <= 0

        return true;
    }

}  // namespace DreamEngine::Core::Math
#endif