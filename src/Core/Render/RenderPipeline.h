#ifndef CORE_RENDER_RENDER_PIPELINE_H_
#define CORE_RENDER_RENDER_PIPELINE_H_

#include "CoreExport.h"
#include "RenderAPI.h"

#include "RenderView.h"
#include <glad/glad.h>  
#include "GLFW/glfw3.h"
#include "GameSystem/Game.h"

namespace DreamEngine::Core::Render
{
class CORE_API RenderPipeline final
{
public:
    void Initialize(RenderAPI* renderer, GLFWwindow* window, const int width, const int height);
    void Render(GameSystem::Game* game, const RenderView& renderView);
private:
    RenderAPI* m_pRenderer = nullptr;
    GLFWwindow* m_pWindow = nullptr;
    std::vector<RenderView> m_renderViews;
    void RenderSceneEntities(Scene* scene, int width, int height) const;
};
}  // namespace DreamEngine::Core::Render
#endif