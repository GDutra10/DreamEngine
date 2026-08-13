#ifndef EDITOR_CONTROLLERS_ENTITY_CONTROLLER_H_
#define EDITOR_CONTROLLERS_ENTITY_CONTROLLER_H_

#include "../../Core/Render/Material.h"
#include "../../Core/ECS/Entity.h"
#include "../Models/Result.h"
#include "../EditorContext.h"

namespace DreamEngine::Editor::Controllers
{
using namespace DreamEngine::Core::ECS;
using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Editor;
using namespace DreamEngine::Editor::Models;

class EntityController
{
   public:
    EntityController(EditorContext& editorContext);
    void AddEntity();
    void DeleteEntity(Entity* entity);
    
   private:
    EditorContext& m_editorContext;
};

}  // namespace DreamEngine::Editor::Controllers
#endif