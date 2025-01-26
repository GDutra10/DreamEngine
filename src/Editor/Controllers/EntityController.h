#ifndef EDITOR_CONTROLLERS_ENTITY_CONTROLLER_H_
#define EDITOR_CONTROLLERS_ENTITY_CONTROLLER_H_

#include "../../Core/Render/Material.h"
#include "../../Core/ECS/Entity.h"
#include "../Models/Result.h"

namespace DreamEngine::Editor::Controllers
{
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Editor::Models;

class EntityController
{
   public:
    EntityController() = default;
    static void DeleteEntity(Entity* entity);
    
   private:
};

}  // namespace DreamEngine::Editor::Controllers
#endif