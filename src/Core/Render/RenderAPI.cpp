#include "RenderAPI.h"
#include "RenderView.h"
#include "../ECS/Components/MeshComponent.h"
#include "ECS/Components/ParentComponent.h"
#include "UI/UiManager.h"

using namespace DreamEngine::Core::Render;

void RenderAPI::AfterRender(RenderView& renderview)
{
    for (std::function<void(RenderView& renderView)>& afterRenderEntitiesCallback : m_afterRenderEntitiesCallbacks)
        afterRenderEntitiesCallback(renderview);
}

void RenderAPI::AddBeforeRenderEntitiesCallbacks(const std::function<void(RenderView& renderView)>& callback)
{
    m_beforeRenderEntitiesCallbacks.push_back(callback);
}

void RenderAPI::AddAfterRenderEntitiesCallbacks(const std::function<void(RenderView& renderView)>& callback)
{
    m_afterRenderEntitiesCallbacks.push_back(callback);
}

void RenderAPI::AddAfterRenderEntityCallbacks(const std::function<void(RenderView& renderView, Entity& entity)>& callback)
{
    m_afterRenderEntityCallbacks.push_back(callback);
}

void RenderAPI::RescaleFrameBuffers(int width, int height) const
{
    for (FrameBuffer* frameBuffer : m_frameBuffers)
    {
        if (frameBuffer != nullptr)
            frameBuffer->Rescale(width, height);
    }
}

std::vector<FrameBuffer*> RenderAPI::GetFrameBuffers() const
{
    return m_frameBuffers;
}

void RenderAPI::BeforeRender(RenderView& renderView)
{
    for (std::function<void(RenderView& renderView)>& beforeRenderEntitiesCallback : m_beforeRenderEntitiesCallbacks)
        beforeRenderEntitiesCallback(renderView);
}