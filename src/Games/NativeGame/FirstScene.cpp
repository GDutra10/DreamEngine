#include "FirstScene.h"
#include <Loggers/LoggerSingleton.h>
#include <IO/File.h>
#include <Resources/ResourceManager.h>
#include <Application.h>
#include <Render/Factories/MeshFactory.h>
#include <Render/Shape.h>
#include <ECS/Components/MeshComponent.h>
#include <ECS/Components/MaterialComponent.h>
#include <ECS/Components/NativeScriptComponent.h>

#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/UiComponent.h"
#include "Vendors/stb_image.h"
#include "Scripts/BoxScript.h"
#include "Scripts/CameraScript.h"
#include "Scripts/HudScript.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::IO;
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Loggers;

void FirstScene::Update(const float deltaTime) 
{
    Scene::Update(deltaTime);
}

void FirstScene::Initialize()
{
    Scene::Initialize();

    m_mustRunScriptComponents = false;

    stbi_set_flip_vertically_on_load(true);

    // set global light
    this->m_globalLight->directionalLight.color = {255, 255, 255};
    this->m_globalLight->directionalLight.influence = 0.5f;
    this->m_globalLight->directionalLight.specular = {255, 100, 100};

    LoadResources();
    CreateEntities();
}

void FirstScene::Unload() 
{
    Scene::Unload();
}

void FirstScene::LoadResources()
{
    // default shader
    const std::string vertexShader = File::ReadAllText("Assets/Shaders/default.vert.glsl");
    const std::string fragmentShader = File::ReadAllText("Assets/Shaders/default.frag.glsl");
    Shader* shader = Application::Instance().GetRenderAPI()->CreateShader("defaultshader", vertexShader, fragmentShader);
    shader->name = "defaultshader";
    ResourceManager::Instance().AddShader(shader->name, shader);

    // default material
    Material* material = new Material();
    material->name = "defaultmaterial";
    material->shader = shader;
    material->specular = {0.5f, 0.5f, 0.5f};
    material->ambient = {0.2f, 0.2f, 0.2f};
    material->diffuse = {0.8f, 0.8f, 0.8f};
    material->shininess = 32.0f;
    ResourceManager::Instance().AddMaterial("defaultmaterial", material);

    // default texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Assets/Textures/defaulttexture.jpg", &width, &height, &nrChannels, 0);
    Texture* texture = DreamEngine::Core::Application::Instance().GetRenderAPI()->CreateTexture(data, width, height, nrChannels);
    texture->name = "defaulttexture";
    stbi_image_free(data);

    if (texture != nullptr)
    {
        texture->type = Diffuse;
        ResourceManager::Instance().AddTexture("defaulttexture", texture);
    }

    Mesh* cubeMesh = DreamEngine::Core::Render::Factories::MeshFactory::CreateMesh(DreamEngine::Core::Render::Shape::Cube);
    cubeMesh->name = "cubemesh";
    ResourceManager::Instance().AddMesh("cubemesh", cubeMesh);

    // ui content
    UiContent* hudUiContent = new UiContent();
    hudUiContent->name = "hud";
    hudUiContent->text = File::ReadAllText("Assets/UI/hud.rml");
    ResourceManager::Instance().AddUiContent("hud_ui_content", hudUiContent);
}

void FirstScene::CreateEntities()
{
    // add box
    Entity* entity = this->m_entityManager->AddEntity("box");
    TransformComponent& transformComponent = entity->GetComponent<TransformComponent>();
    transformComponent.has = true;
    transformComponent.SetPosition({0.0f, 0, -10.0f});

    MeshComponent& meshComponent = entity->GetComponent<MeshComponent>();
    meshComponent.has = true;
    meshComponent.mesh = ResourceManager::Instance().GetMesh("cubemesh");

    MaterialComponent& materialComponent = entity->GetComponent<MaterialComponent>();
    materialComponent.has = true;
    materialComponent.material = ResourceManager::Instance().GetMaterial("defaultmaterial");

    NativeScriptComponent& nativeScriptComponent = entity->GetComponent<NativeScriptComponent>();
    nativeScriptComponent.has = true;
    nativeScriptComponent.script = new BoxScript();

    // add camera
    Entity* cameraEntity = this->m_entityManager->AddEntity("main_camera");
    CameraComponent& cameraComponent = cameraEntity->GetComponent<CameraComponent>();
    cameraComponent.has = true;

    NativeScriptComponent& cameraScriptComponent = cameraEntity->GetComponent<NativeScriptComponent>();
    cameraScriptComponent.has = true;
    cameraScriptComponent.script = new CameraScript();

    this->SetMainCameraEntity(cameraEntity);

    // add ui
    Entity* uiEntity = this->m_entityManager->AddEntity("ui_canvas");
    UiComponent& uiComponent = uiEntity->GetComponent<UiComponent>();
    uiComponent.has = true;
    uiComponent.content = ResourceManager::Instance().GetUiContent("hud_ui_content");

    NativeScriptComponent& uiScriptComponent = uiEntity->GetComponent<NativeScriptComponent>();
    uiScriptComponent.has = true;
    uiScriptComponent.script = new HudScript();
}
