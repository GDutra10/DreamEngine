#include "EntityController.h"

#include "../Singletons/EditorSingleton.h"
#include "Loggers/LoggerSingleton.h"
#include "../Models/Datas/EntityConfigData.h"

using namespace DreamEngine::Editor::Models::Datas;
using namespace DreamEngine::Editor::Controllers;
using namespace DreamEngine::Editor::Singletons;

void EntityController::AddEntity()
{
    LoggerSingleton::Instance().LogInfo("Add new Entity");

    EntityManager* entityManager = EditorSingleton::Instance().GetEntityManager();
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

    EditorSingleton::Instance().sceneData->entities.push_back(entityConfig);
}

void EntityController::DeleteEntity(Entity* entity)
{
    std::vector<EntityConfigData> entities = EditorSingleton::Instance().sceneData->entities;

    std::erase_if(entities, [entity](const EntityConfigData& e)
    {
        return e.identifier == entity->GetIdentifier();
    });

    if (entity->GetIdentifier() == EditorSingleton::Instance().GetEditorScene()->GetMainCameraEntity()->GetIdentifier())
        EditorSingleton::Instance().GetEditorScene()->SetMainCameraEntity(nullptr);

    EditorSingleton::Instance().SetSelectedEntity(nullptr);
    EditorSingleton::Instance().GetEntityManager()->RemoveEntity(entity);
}
