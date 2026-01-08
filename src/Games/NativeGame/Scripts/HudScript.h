#include <Scripting/NativeScript.h>

#include "ECS/Components/UiComponent.h"

using namespace DreamEngine::Core::Scripting;
using namespace DreamEngine::Core::UI;

class HudScript : public NativeScript
{
   public:
    void Update() override;

   protected:
    void Start() override;

   private:
    int m_health = 100;
    int m_ammo = 999;
    float m_degree = 90.0;
    std::string m_name = "John";
    UiComponent* m_pUiComponent = nullptr;
};