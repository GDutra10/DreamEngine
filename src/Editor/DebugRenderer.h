#ifndef EDITOR_DEBUG_RENDERER_H_
#define EDITOR_DEBUG_RENDERER_H_

#include <vector>

#include "../Vendors/glm/glm/glm.hpp"
#include "../Core/GameSystem/Scene.h"
#include "../Core/Math/AABB.h"
#include "../Core/Render/RenderView.h"
#include "../Core/Render/RenderAPI.h"
#include "../Core/Render/Mesh.h"

namespace DreamEngine::Editor
{
    using namespace DreamEngine::Core::GameSystem;
    using namespace DreamEngine::Core::Render;
    using namespace DreamEngine::Core::Math;

    struct DebugRenderContext
    {
        Scene& scene;
        RenderView& renderView;
        RenderAPI* renderAPI;
        Entity* entitySelected;
    };

    struct RenderOutlineContext
    {
        DebugRenderContext& debugRenderContext;
        Camera& camera;
        glm::mat4& view;
        glm::mat4& projection;
        std::vector<Entity*> outlinedChildren;
    };

class DebugRenderer final
{
   public:
    void Render(DebugRenderContext& context);
   private:
    Mesh* m_wireCube = nullptr;
    void RenderAABB(const AABB& bounds, glm::mat4& view, glm::mat4 & projection, const glm::vec3& color);
    void RenderOutline(Entity* entity, RenderOutlineContext context);
};

}  // namespace DreamEngine::Editor
#endif