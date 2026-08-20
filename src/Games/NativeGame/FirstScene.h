#include <GameSystem/Scene.h>
#include <vector>
#include <cstdint>

using namespace DreamEngine::Core::GameSystem;

class FirstScene : public Scene
{
   public:
    FirstScene(std::string name) : Scene(name) {}
    void Update(const float deltaTime) override;
    void Initialize() override;
    void Unload() override;
   private:
    void CreateEntities();
    static void LoadResources();
    static std::vector<uint8_t> ReadBinaryFile(const std::string& path);
    static AudioClip* LoadFromFile(const std::string& path);
};