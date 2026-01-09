#include "ECS/Components/TransformComponent.h"
#include "Scripting/NativeScript.h"

using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::Scripting;

class CameraScript : public NativeScript
{
public:
    void Update() override;
protected:
	void Start() override;
private:
    TransformComponent* m_pTransformComponent = nullptr;
    float m_speed = 0.05f;
};