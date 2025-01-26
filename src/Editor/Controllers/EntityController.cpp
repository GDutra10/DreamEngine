#include "EntityController.h"

#include "../Singletons/EditorSingleton.h"

using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Singletons;

void EntityController::DeleteEntity(Entity* entity)
{
    EditorSingleton::Instance().SetSelectedEntity(nullptr);
    EditorSingleton::Instance().GetEntityManager()->RemoveEntity(entity);
}
