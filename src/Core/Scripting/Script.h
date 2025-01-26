#ifndef CORE_SCRIPTING_SCRIPT_H_
#define CORE_SCRIPTING_SCRIPT_H_

#include <string>

#include "../Resources/Resource.h"
#include "../Sync/EntityData.h"

namespace DreamEngine::Core::Scripting
{
using namespace DreamEngine::Core::Resources;
using namespace DreamEngine::Core::Sync;
class Script : public Resource
{
   public:
    Script(const std::string& className, const std::string& assemblyName);
    void* Update(void* instance, EntityData* entityData);
    void* Destroy(void* instance);
    std::string GetAssemblyName() const;
    std::string GetClassName() const;

   private:
    std::string m_className;
    std::string m_assemblyName;
};
}  // namespace DreamEngine::Core::Scripts
#endif