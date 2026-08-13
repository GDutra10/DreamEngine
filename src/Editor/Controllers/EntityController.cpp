#include "EntityController.h"

#include "Loggers/LoggerSingleton.h"
#include "../Models/Datas/EntityConfigData.h"
#include "../../Core/Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor::Models::Datas;
using namespace DreamEngine::Editor::Controllers;

EntityController::EntityController(EditorContext& editorContext) : m_editorContext(editorContext) {}

void EntityController::AddEntity()
{
    LoggerSingleton::Instance().LogInfo("Add new Entity");

    EntityManager* entityManager = m_editorContext.GetEntityManager();
    Entity* entity = entityManager->AddEntity("entity");
    EntityConfigData entityConfig;

    entityConfig.identifier = entity->GetIdentifier();
    entityConfig.name = entity->GetName();
    entityConfig.tag = entity->GetTag();
    entityConfig.isActive = entity->GetIsActive();

    TransformComponent& transform = entity->GetComponent<TransformComponent>();
    const glm::vec3& position = transform.GetPosition();
    const glm::vec3& scale = transform.GetScale();
    const glm::vec3& rotation = transform.GetRotation();

    entityConfig.transform.position.x = position.x;
    entityConfig.transform.position.y = position.y;
    entityConfig.transform.position.z = position.z;
    entityConfig.transform.scale.x = scale.x;
    entityConfig.transform.scale.y = scale.y;
    entityConfig.transform.scale.z = scale.z;
    entityConfig.transform.rotation.x = rotation.x;
    entityConfig.transform.rotation.y = rotation.y;
    entityConfig.transform.rotation.z = rotation.z;

    m_editorContext.GetSceneData()->entities.push_back(entityConfig);
}

void EntityController::DeleteEntity(Entity* entity)
{
    std::vector<EntityConfigData> entities = m_editorContext.GetSceneData()->entities;

    std::erase_if(entities, [entity](const EntityConfigData& e)
    {
        return e.identifier == entity->GetIdentifier();
    });

    if (entity->GetIdentifier() == m_editorContext.GetEditorScene()->GetMainCameraEntity()->GetIdentifier())
        m_editorContext.GetEditorScene()->SetMainCameraEntity(nullptr);

    m_editorContext.SetSelectedEntity(nullptr);
    m_editorContext.GetEntityManager()->RemoveEntity(entity);
}
