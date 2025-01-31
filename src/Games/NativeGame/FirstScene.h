#include <GameSystem/Scene.h>

using namespace DreamEngine::Core::GameSystem;

class FirstScene : public Scene
{
   public:
    FirstScene(std::string name) : Scene(name) {}
    void Update(const float deltaTime) override;
    void Initialize() override;
    void Unload() override;
};