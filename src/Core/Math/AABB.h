#ifndef CORE_MATH_AABB_H
#define CORE_MATH_AABB_H

#include "../../Vendors/glm/glm/glm.hpp"
#include "CoreExport.h"

namespace DreamEngine::Core::Math
{
	struct CORE_API AABB
	{
        glm::vec3 min{0.0f};
        glm::vec3 max{0.0f};

        [[nodiscard]] static AABB FromCenterSize(const glm::vec3& center, const glm::vec3& size) noexcept
        {
            const glm::vec3 halfSize = size * 0.5f;

            return {center - halfSize, center + halfSize};
        }

		[[nodiscard]] bool Intersects(const AABB& other) const noexcept
		{
            if (max.x < other.min.x || min.x > other.max.x)
                return false;

            if (max.y < other.min.y || min.y > other.max.y)
                return false;

            if (max.z < other.min.z || min.z > other.max.z)
                return false;

            return true;
		}

        /*[[nodiscard]] glm::vec3 GetCenter() const noexcept;
        [[nodiscard]] glm::vec3 GetExtents() const noexcept;*/
	};
} // namespace DreamEngine::Core::Math
#endif