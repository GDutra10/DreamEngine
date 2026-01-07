#include "RenderAPI.h"
#include "OutlineScope.h"
#include "../ECS/Components/MeshComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../ECS/Components/DirectionalLightComponent.h"
#include "../ECS/Components/MaterialComponent.h"
#include "../Math/Math.h"
#include "ECS/Components/ParentComponent.h"
#include "ECS/Components/OutlineComponent.h"
#include "UI/UiManager.h"

using namespace DreamEngine::Core::Render;

void RenderAPI::Render(Game* game)
{
    Scene* currentScene = game->GetActiveScene();
    Color* color = currentScene->GetBackgroundColor();
    const std::vector<Entity*>& entities = currentScene->GetEntityManager()->GetEntities();

    SetSceneBackgroundColor(color);
    
    if (entities.empty())
        return;

    // view and projection from camera
    Camera& camera = currentScene->GetCamera();
    camera.SetPerspectiveProjectionMatrix(
        glm::radians(camera.fovDegree),
        static_cast<float>(game->width), 
        static_cast<float>(game->height),
        camera.near, 
        camera.far);
    glm::mat4 view = camera.GetView();
    glm::mat4 projection = camera.GetProjection();

    // global light position
    GlobalLight* globalLight = currentScene->GetGlobalLight();
    const glm::vec3 globalLightPosition = globalLight->transform.GetPosition();
    DirectionalLightComponent& directionalLightComponent = globalLight->directionalLight;
    // light properties
    const glm::vec3 diffuseColor = directionalLightComponent.color.ToVec3() * glm::vec3(directionalLightComponent.influence);  // decrease the influence
    const glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

    // render all entities
    for (Entity* entity : entities)
    {
        if (!entity->GetIsActive() || 
            !entity->GetComponent<MeshComponent>().has ||
            !entity->GetComponent<MaterialComponent>().has)
            continue;

        const MeshComponent& meshComponent = entity->GetComponent<MeshComponent>();
        const MaterialComponent& materialComponent = entity->GetComponent<MaterialComponent>();
        OutlineComponent& outlineComponent = entity->GetComponent<OutlineComponent>();
        const bool wantsOutline = outlineComponent.has && outlineComponent.shader != nullptr;

        // is this correct?
        if (meshComponent.mesh == nullptr || materialComponent.material == nullptr)
            continue;

        StencilDefaultNoWrite();
        
        if (wantsOutline)
            StencilWriteObject();

        const Material* material = materialComponent.material;
        Shader* shader = material->shader;
        shader->Use();

        // set view position
        shader->SetVec3("viewPos", camera.position);

        shader->SetVec3("light.position", globalLightPosition);
        shader->SetVec3("light.ambient", ambientColor);
        shader->SetVec3("light.diffuse", diffuseColor);
        shader->SetVec3("light.specular", directionalLightComponent.specular);

        // material
        shader->SetVec3("material.ambient", material->ambient);
        shader->SetVec3("material.diffuse", material->diffuse);
        shader->SetVec3("material.specular", material->specular);
        shader->SetFloat("material.shininess", material->shininess);

        // pass projection matrix to shader (note that in this case it could change every frame)
        shader->SetMat4("projection", projection);

        // camera/view transformation
        shader->SetMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 transform = entity->GetTransform();
        shader->SetMat4("model", transform);

        meshComponent.mesh->Draw(*shader);

        for (const std::function<void(Entity* e)>& func : m_afterRenderEntityCallbacks)
            func(entity);

        if (wantsOutline)
        {
            // draw where stencil != 1
            StencilDrawOutlineRegion();

            OutlineOptions opts{};
            opts.disableDepthTest = false;
            opts.cullFace = OutlineOptions::CullFace::Front;

            OutlineScope guard(*this, opts);

            outlineComponent.shader->Use();
            outlineComponent.shader->SetMat4("view", view);
            outlineComponent.shader->SetVec3("viewPos", camera.position);
            outlineComponent.shader->SetMat4("projection", projection);
            outlineComponent.shader->SetMat4("model", transform);
            outlineComponent.shader->SetVec3("outlineColor", outlineComponent.color.ToVec3());
            outlineComponent.shader->SetFloat("thicknessWS", outlineComponent.thickness);

            meshComponent.mesh->Draw(*outlineComponent.shader);

            // reset stencil behavior
            StencilDefaultNoWrite();
        }
    }
}

void RenderAPI::AfterRender(int width, int height)
{
    for (std::function<void(int w, int h)>&afterRenderEntitiesCallback : m_afterRenderEntitiesCallbacks)
        afterRenderEntitiesCallback(width, height);
}

void RenderAPI::AddBeforeRenderEntitiesCallbacks(const std::function<void()>& callback)
{
    m_beforeRenderEntitiesCallbacks.push_back(callback);
}

void RenderAPI::AddAfterRenderEntitiesCallbacks(const std::function<void(const int width, const int height)>& callback)
{
    m_afterRenderEntitiesCallbacks.push_back(callback);
}

void RenderAPI::AddAfterRenderEntityCallbacks(const std::function<void(Entity* entity)>& callback)
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

void RenderAPI::BeforeRender()
{
    for (std::function<void()>& beforeRenderEntitiesCallback : m_beforeRenderEntitiesCallbacks)
        beforeRenderEntitiesCallback();
}
