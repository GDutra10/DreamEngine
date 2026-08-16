#ifndef CORE_GAME_SYSTEM_PRE_FAB_H_
#define CORE_GAME_SYSTEM_PRE_FAB_H_

#include <map>
#include <string>
#include "../CoreExport.h"
#include "../Resources/Resource.h"
#include "../GameSystem/Definitions/PrefabEntityDefinition.h"

namespace DreamEngine::Core::GameSystem
{
	using namespace DreamEngine::Core::Resources;
	using namespace DreamEngine::Core::GameSystem::Definitions;
class CORE_API Prefab : public Resource
{
   public:
    PrefabEntityDefinition root;
   private:
};
}  // namespace DreamEngine::Core::GameSystem
#endif