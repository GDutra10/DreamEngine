#include "DebugRenderer.h"

#include "EditorDefine.h"

#include "../Core/Render/Factories/MeshFactory.h"
#include "../Core/ECS/Components/MeshComponent.h"
#include "../Core/ECS/Components/MaterialComponent.h"
#include "../Core/ECS/Components/ChildrenComponent.h"
#include "../Core/Render/OutlineScope.h"
#include "../Vendors/glm/glm/glm.hpp"

using namespace DreamEngine::Editor;
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::Render::Factories;

void DebugRenderer::Render(DebugRenderContext& context)
{
    if (m_wireCube == nullptr)
        m_wireCube = MeshFactory::CreateMesh(Shape::WireCube, PrimitiveTopology::Lines);

    const std::vector<Entity*>& entities = context.scene.GetEntityManager()->GetEntities();

    Camera& camera = context.scene.GetCamera();
    glm::mat4 view = camera.GetView();
    glm::mat4 projection = camera.GetProjection();
    std::vector<Entity*> outlinedChildren;
    RenderOutlineContext outlineContext = {
        context, camera, view, projection, outlinedChildren,
    };


    for (Entity* entity : entities)
    {
        if (!entity->GetIsActive())
            continue;

        const ColliderComponent& collider = entity->GetComponent<ColliderComponent>();
        
        if (collider.has && collider.enabled)
            RenderAABB(collider.worldBounds, view, projection, glm::vec3{0.2f, 1.0f, 0.2f});

        if (entity->GetComponent<MeshComponent>().has && entity->GetComponent<MaterialComponent>().has)
            RenderOutline(entity, outlineContext);
    }
}

void DebugRenderer::RenderAABB(const AABB& bounds, glm::mat4& view, glm::mat4& projection, const glm::vec3& color)
{
    const glm::vec3 center = (bounds.min + bounds.max) * 0.5f;
    const glm::vec3 size = bounds.max - bounds.min;

    glm::mat4 model{1.0f};
    model = glm::translate(model, center);
    model = glm::scale(model, size);

    Shader* shader = ResourceManager::Instance().GetShader(EDITOR_OUTLINE_SHADER_NAME);

    shader->Use();
    shader->SetMat4("view", view);
    shader->SetMat4("projection", projection);
    shader->SetMat4("model", model);
    shader->SetVec3("outlineColor", color);

    m_wireCube->Draw(*shader);
}

void DebugRenderer::RenderOutline(Entity* entity, RenderOutlineContext context)
{
    const MeshComponent& meshComponent = entity->GetComponent<MeshComponent>();
    std::vector<Entity*>::iterator it = std::find(context.outlinedChildren.begin(), context.outlinedChildren.end(), entity);
    glm::mat4 transform = entity->GetWorldTransform();

    if (entity == context.debugRenderContext.entitySelected || it != context.outlinedChildren.end())
    {
        const ChildrenComponent& childrenComponent = entity->GetComponent<ChildrenComponent>();

        if (childrenComponent.has)
        {
            for (Entity* child : childrenComponent.children)
                context.outlinedChildren.push_back(child);
        }

        context.debugRenderContext.renderAPI->StencilWriteObject();

        // draw where stencil != 1
        context.debugRenderContext.renderAPI->StencilDrawOutlineRegion();

        OutlineOptions opts{};
        opts.disableDepthTest = false;
        opts.cullFace = OutlineOptions::CullFace::Front;

        DreamEngine::Core::Render::OutlineScope guard(*context.debugRenderContext.renderAPI, opts);

        Shader* outlineShader = ResourceManager::Instance().GetShader(EDITOR_OUTLINE_SHADER_NAME);
        outlineShader->Use();
        outlineShader->SetMat4("view", context.view);
        outlineShader->SetVec3("viewPos", context.camera.position);
        outlineShader->SetMat4("projection", context.projection);
        outlineShader->SetMat4("model", transform);
        outlineShader->SetVec3("outlineColor", {1.f, 1.f, 0});
        outlineShader->SetFloat("thicknessWS", 0.02f);

        meshComponent.mesh->Draw(*outlineShader);

        // reset stencil behavior
        context.debugRenderContext.renderAPI->StencilDefaultNoWrite();
    }
}
