#include "FirstScene.h"
#include <Loggers/LoggerSingleton.h>
#include <IO/File.h>
#include <Resources/GlobalResourceManager.h>
#include <Application.h>
#include <Render/Factories/MeshFactory.h>
#include <Render/Shape.h>
#include <ECS/Components/MeshComponent.h>
#include <ECS/Components/MaterialComponent.h>
#include <ECS/Components/NativeScriptComponent.h>

#include "Vendors/stb_image.h"
#include "Scripts/BoxScript.h"

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

    // default shader
    const std::string vertexShader = File::ReadAllText("Assets/Shaders/default.vert.glsl");
    const std::string fragmentShader = File::ReadAllText("Assets/Shaders/default.frag.glsl");
    Shader* shader = Application::Instance().GetRenderAPI()->CreateShader("defaultshader", vertexShader, fragmentShader);
    shader->name = "defaultshader";
    GlobalResourceManager::Instance().AddShader(shader->name, shader);

    // default material
    Material* material = new Material();
    material->name = "defaultmaterial";
    material->shader = shader;
    material->specular = {0.5f, 0.5f, 0.5f};
    material->ambient = {0.2f, 0.2f, 0.2f};
    material->diffuse = {0.8f, 0.8f, 0.8f};
    material->shininess = 32.0f;
    GlobalResourceManager::Instance().AddMaterial("defaultmaterial", material);

    // default texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Assets/Textures/defaulttexture.jpg", &width, &height, &nrChannels, 0);
    Texture* texture = DreamEngine::Core::Application::Instance().GetRenderAPI()->CreateTexture(data, width, height, nrChannels);
    texture->name = "defaulttexture";
    stbi_image_free(data);

    if (texture != nullptr)
    {
        texture->type = Diffuse;
        GlobalResourceManager::Instance().AddTexture("defaulttexture", texture);
    }

    Mesh* cubeMesh = DreamEngine::Core::Render::Factories::MeshFactory::CreateMesh(DreamEngine::Core::Render::Shape::Cube);
    cubeMesh->name = "cubemesh";
    GlobalResourceManager::Instance().AddMesh("cubemesh", cubeMesh);

    // add entities
    Entity* entity = this->m_entityManager->AddEntity("player");
    TransformComponent& transformComponent = entity->GetComponent<TransformComponent>();
    transformComponent.has = true;
    transformComponent.SetPosition({-1.0f, -2.0f, -10.0f});

    MeshComponent& meshComponent = entity->GetComponent<MeshComponent>();
    meshComponent.has = true;
    meshComponent.mesh = cubeMesh;
    
    MaterialComponent& materialComponent = entity->GetComponent<MaterialComponent>();
    materialComponent.has = true;
    materialComponent.material = material;

    NativeScriptComponent& nativeScriptComponent = entity->GetComponent<NativeScriptComponent>();
    nativeScriptComponent.has = true;
    nativeScriptComponent.script = new BoxScript();
}

void FirstScene::Unload() 
{
    Scene::Unload();
}
