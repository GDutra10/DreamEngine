#ifndef CORE_ECS_ENTITY_H_
#define CORE_ECS_ENTITY_H_

#include <string>
#include "Components/Component.h"
#include "Sync/EntityData.h"
#include "glm/fwd.hpp"
#include "CoreExport.h"

namespace DreamEngine::Core::ECS
{

using namespace DreamEngine::Core::ECS::Components;
using namespace DreamEngine::Core::Sync;

class CORE_API Entity
{
    friend class EntityManager;

   public:
    EntityData entityData;
    void Destroy() const;
    bool IsActive() const;
    size_t GetId() const;
    std::string& GetTag();
    std::string& GetName();
    void SetActive(const bool value) const;
    // get the transform by propagation when need it
    glm::mat4 GetTransform();

    template <IsComponent T>
    T& GetComponent();

    template <IsComponent T>
    bool HasComponent() const;

   private:
    size_t m_id;
    Entity(const size_t& id) : m_id(id) {}
};
}
#endif