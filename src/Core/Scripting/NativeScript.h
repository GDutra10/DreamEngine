#ifndef CORE_SCRIPTING_NATIVE_SCRIPT_H_
#define CORE_SCRIPTING_NATIVE_SCRIPT_H_

#include <string>
#include "../ECS/Entity.h"

namespace DreamEngine::Core::Scripting
{
using namespace DreamEngine::Core::ECS;
class NativeScript
{
   public:
    Entity* entity = nullptr;
    NativeScript() {};
    void Initialize();
    virtual void Update() {};

    // Events
    virtual void OnTriggerEnter(Entity* other) {}
    virtual void OnTriggerStay(Entity* other) {}
    virtual void OnTriggerExit(Entity* other) {}

    virtual void OnCollisionEnter(Entity* other) {}
    virtual void OnCollisionStay(Entity* other) {}
    virtual void OnCollisionExit(Entity* other) {}

   protected:
    virtual void Start() {}
    
   private:
    bool m_wasInitialized = false;
};
}  // namespace DreamEngine::Core::Scripting
#endif