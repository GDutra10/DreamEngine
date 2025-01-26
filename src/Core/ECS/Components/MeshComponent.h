#ifndef CORE_ECS_COMPONENTS_MESH_COMPONENT_H_
#define CORE_ECS_COMPONENTS_MESH_COMPONENT_H_

#include "../../Core/Render/Mesh.h"
#include "Component.h"

namespace DreamEngine::Core::ECS::Components
{
using namespace DreamEngine::Core::Render;

class MeshComponent : public Component
{
   public:
    Mesh* mesh;

    explicit MeshComponent() = default;
    MeshComponent(Mesh* mesh) : mesh(mesh)  {}
};
}  // namespace DreamEngine::Core::ECS::Components
#endif