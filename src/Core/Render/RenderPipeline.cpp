#include "RenderPipeline.h"

#include "OutlineScope.h"
#include "RenderViewProvider.h"
#include "ECS/Components/MaterialComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "Loggers/LoggerSingleton.h"
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

void RenderPipeline::Render(Scene* scene, RenderView& renderView)
{
    if (m_pRenderer == nullptr)
        throw std::runtime_error("render pipeline not initialized!");

    if (renderView.frameBuffer != nullptr)
        renderView.frameBuffer->Attach();
    
    m_pRenderer->BeforeRender(renderView);
    m_pRenderer->SetSceneBackgroundColor(scene->GetBackgroundColor());

    if (renderView.mask & RenderMask::World)
        RenderSceneEntities(renderView, scene);
    
    if (renderView.mask & RenderMask::UI)
    {
        UiManager::BeginRender(renderView.width, renderView.height);
        UiManager::Render();
        UiManager::EndRender();
    }

    if (renderView.mask & RenderMask::Debug && m_fnDebugPass)
        m_fnDebugPass(*scene, renderView, m_pRenderer);

    m_pRenderer->AfterRender(renderView);

    if (renderView.frameBuffer != nullptr)
        renderView.frameBuffer->Detach();
}

void RenderPipeline::RenderSceneEntities(RenderView& renderView, Scene* scene) const
{
    const std::vector<Entity*>& entities = scene->GetEntityManager()->GetEntities();

    if (entities.empty())
        return;

    // view and projection from camera
    Camera& camera = scene->GetCamera();
    //camera.SetPerspectiveProjectionMatrix(glm::radians(camera.fovDegree), static_cast<float>(renderView.width), static_cast<float>(renderView.height), camera.near, camera.far);

    float aspect = (renderView.height > 0) ? (float)renderView.width / (float)renderView.height : 1.0f;
    camera.SetPerspective(glm::radians(camera.fovDegree), aspect, camera.near, camera.far);
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
        
        // is this correct?
        if (meshComponent.mesh == nullptr || materialComponent.material == nullptr)
            continue;

        m_pRenderer->StencilDefaultNoWrite();

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

        /*for (const std::function<void(RenderView& renderView, Entity& entity)>& func : m_pRenderer->))
            func(renderView, *entity);*/
    }
}