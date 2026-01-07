#include "RenderPipeline.h"

#include "OutlineScope.h"
#include "RenderViewProvider.h"
#include "ECS/Components/MaterialComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/OutlineComponent.h"
#include "UI/UiManager.h"

using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Core::GameSystem;

void RenderPipeline::Initialize(RenderAPI* renderer, GLFWwindow* window, const int width, const int height)
{
    m_pWindow = window;
    m_pRenderer = renderer;
    m_pRenderer->Initialize(width, height);
    UiManager::Initialize(window, width, height);
    RenderViewProvider::Initialize();
}

void RenderPipeline::Render(GameSystem::Game* game, const RenderView& renderView)
{
    if (m_pRenderer == nullptr)
        throw std::runtime_error("render pipeline not initialized!");

    if (renderView.frameBuffer != nullptr)
        renderView.frameBuffer->Attach();

    Scene* currentScene = game->GetActiveScene();
    Color* color = currentScene->GetBackgroundColor();

    m_pRenderer->SetSceneBackgroundColor(color);

    if (renderView.mask & RenderMask::World)
        RenderSceneEntities(currentScene, game->width, game->height);
    
    if (renderView.mask & RenderMask::UI)
    {
        UiManager::BeginRender(game);
        UiManager::Render(game);
        UiManager::EndRender();
    }

    if (renderView.frameBuffer != nullptr)
        renderView.frameBuffer->Detach();
}

void RenderPipeline::RenderSceneEntities(Scene* scene, const int width, const int height) const
{
    const std::vector<Entity*>& entities = scene->GetEntityManager()->GetEntities();

    if (entities.empty())
        return;

    // view and projection from camera
    Camera& camera = scene->GetCamera();
    camera.SetPerspectiveProjectionMatrix(glm::radians(camera.fovDegree), static_cast<float>(width), static_cast<float>(height), camera.near, camera.far);
    glm::mat4 view = camera.GetView();
    glm::mat4 projection = camera.GetProjection();

    // global light position
    GlobalLight* globalLight = scene->GetGlobalLight();
    const glm::vec3 globalLightPosition = globalLight->transform.GetPosition();
    DirectionalLightComponent& directionalLightComponent = globalLight->directionalLight;
    
    // light properties
    const glm::vec3 diffuseColor = directionalLightComponent.color.ToVec3() * glm::vec3(directionalLightComponent.influence);  // decrease the influence
    const glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

    // render all entities
    for (Entity* entity : entities)
    {
        if (!entity->GetIsActive() || !entity->GetComponent<MeshComponent>().has || !entity->GetComponent<MaterialComponent>().has)
            continue;

        const MeshComponent& meshComponent = entity->GetComponent<MeshComponent>();
        const MaterialComponent& materialComponent = entity->GetComponent<MaterialComponent>();
        OutlineComponent& outlineComponent = entity->GetComponent<OutlineComponent>();
        const bool wantsOutline = outlineComponent.has && outlineComponent.shader != nullptr;

        // is this correct?
        if (meshComponent.mesh == nullptr || materialComponent.material == nullptr)
            continue;

        m_pRenderer->StencilDefaultNoWrite();

        if (wantsOutline)
            m_pRenderer->StencilWriteObject();

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

        /*for (const std::function<void(Entity* e)>& func : m_pRenderer->GetAfterRenderEntityCallbacks())
            func(entity);*/

        if (wantsOutline)
        {
            // draw where stencil != 1
            m_pRenderer->StencilDrawOutlineRegion();

            OutlineOptions opts{};
            opts.disableDepthTest = false;
            opts.cullFace = OutlineOptions::CullFace::Front;

            OutlineScope guard(*m_pRenderer, opts);

            outlineComponent.shader->Use();
            outlineComponent.shader->SetMat4("view", view);
            outlineComponent.shader->SetVec3("viewPos", camera.position);
            outlineComponent.shader->SetMat4("projection", projection);
            outlineComponent.shader->SetMat4("model", transform);
            outlineComponent.shader->SetVec3("outlineColor", outlineComponent.color.ToVec3());
            outlineComponent.shader->SetFloat("thicknessWS", outlineComponent.thickness);

            meshComponent.mesh->Draw(*outlineComponent.shader);

            // reset stencil behavior
            m_pRenderer->StencilDefaultNoWrite();
        }
    }
}