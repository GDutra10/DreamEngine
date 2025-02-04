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

   protected:
    virtual void Start() {}
    
   private:
    bool m_wasInitialized = false;
};
}  // namespace DreamEngine::Core::Scripting
#endif