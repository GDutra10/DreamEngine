#ifndef CORE_RENDER_RENDER_PIPELINE_H_
#define CORE_RENDER_RENDER_PIPELINE_H_

#include "CoreExport.h"
#include "RenderAPI.h"

#include "RenderPass.h"
#include "RenderView.h"
#include <glad/glad.h>  
#include "GLFW/glfw3.h"
#include "GameSystem/Game.h"

namespace DreamEngine::Core::Render
{
class CORE_API RenderPipeline final : public RenderPass
{
public:
    void Initialize(RenderAPI* renderer, GLFWwindow* window, const int width, const int height);
    void Render(Scene* scene, RenderView& renderView);

   private:
    RenderAPI* m_pRenderer = nullptr;
    GLFWwindow* m_pWindow = nullptr;
    std::vector<RenderView> m_renderViews;
    void RenderSceneEntities(RenderView& renderView, Scene* scene) const;
};
}  // namespace DreamEngine::Core::Render
#endif