#include "EntityController.h"

#include "Loggers/LoggerSingleton.h"
#include "../../Core/GameSystem/Definitions/EntityDefinition.h"
#include "../../Core/Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Core::GameSystem::Definitions;
using namespace DreamEngine::Editor::Controllers;

EntityController::EntityController(EditorContext& editorContext) : m_editorContext(editorContext) {}

void EntityController::AddEntity()
{
    LoggerSingleton::Instance().LogInfo("Add new Entity");

    EntityManager* entityManager = m_editorContext.GetEntityManager();
    Entity* entity = entityManager->AddEntity("entity");
    EntityDefinition entityDefinition;

    entityDefinition.identifier = entity->GetIdentifier();
    entityDefinition.name = entity->GetName();
    entityDefinition.tag = entity->GetTag();
    entityDefinition.isActive = entity->GetIsActive();

    TransformComponent& transform = entity->GetComponent<TransformComponent>();
    const glm::vec3& position = transform.GetPosition();
    const glm::vec3& scale = transform.GetScale();
    const glm::vec3& rotation = transform.GetRotation();

    entityDefinition.transform.position.x = position.x;
    entityDefinition.transform.position.y = position.y;
    entityDefinition.transform.position.z = position.z;
    entityDefinition.transform.scale.x = scale.x;
    entityDefinition.transform.scale.y = scale.y;
    entityDefinition.transform.scale.z = scale.z;
    entityDefinition.transform.rotation.x = rotation.x;
    entityDefinition.transform.rotation.y = rotation.y;
    entityDefinition.transform.rotation.z = rotation.z;

    m_editorContext.GetSceneData()->entities.push_back(entityDefinition);
}

void EntityController::DeleteEntity(Entity* entity)
{
    std::vector<EntityDefinition> definitions = m_editorContext.GetSceneData()->entities;

    std::erase_if(definitions, [entity](const EntityDefinition& e)
    {
        return e.identifier == entity->GetIdentifier();
    });

    if (entity->GetIdentifier() == m_editorContext.GetEditorScene()->GetMainCameraEntity()->GetIdentifier())
        m_editorContext.GetEditorScene()->SetMainCameraEntity(nullptr);

    m_editorContext.SetSelectedEntity(nullptr);
    m_editorContext.GetEntityManager()->RemoveEntity(entity);
}
