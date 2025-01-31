#include "FirstScene.h"
#include <Loggers/LoggerSingleton.h>
#include <IO/File.h>
#include <Resources/GlobalResourceManager.h>
#include <Application.h>

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::IO;
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Loggers;

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

static Direction direction = Direction::UP;

void FirstScene::Update(const float deltaTime) 
{
    auto* backgroundColor = this->GetBackgroundColor();

    if (backgroundColor == nullptr)
    {
        LoggerSingleton::Instance().LogError("Background color is null.");
        return;
    }

    if (backgroundColor->red < 255.0f && direction == Direction::UP)
        backgroundColor->red += 1.0f;
    else if (backgroundColor->red >= 255.0f && direction == Direction::UP)
        direction = Direction::DOWN;
    else if (backgroundColor->red > 0.0f && direction == Direction::DOWN)
        backgroundColor->red = -1.0f;
    else if (backgroundColor->red <= 0.0f && direction == Direction::DOWN)
        direction = Direction::UP;

    Scene::Update(deltaTime);
}

void FirstScene::Initialize() 
{
    Scene::Initialize();

    m_mustRunScriptComponents = false;

    const std::string vertexShader = File::ReadAllText("Assets/Shaders/default.vert.glsl");
    const std::string fragmentShader = File::ReadAllText("Assets/Shaders/default.frag.glsl");
    Shader* shader = Application::Instance().GetRenderAPI()->CreateShader("defaultshader", vertexShader, fragmentShader);
    shader->name = "defaultshader";
    GlobalResourceManager::Instance().AddShader(shader->name, shader);

    this->m_entityManager->AddEntity("player");
}

void FirstScene::Unload() 
{
    Scene::Unload();
}
