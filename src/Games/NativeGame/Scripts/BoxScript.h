#include <Scripting/NativeScript.h>
#include <ECS/Components/TransformComponent.h>

using namespace DreamEngine::Core::Scripting;

class BoxScript : public NativeScript
{
   public:
    void Update() override;
   protected:
    void Start() override;
   private:
    TransformComponent* m_transformComponent = nullptr;
};