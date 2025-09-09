#ifndef CORE_RENDER_FACTORIES_MESH_FACTORIES_H_
#define CORE_RENDER_FACTORIES_MESH_FACTORIES_H_

#include "../Mesh.h"
#include "../Shape.h"

#include "CoreExport.h"

namespace DreamEngine::Core::Render::Factories
{
    using namespace DreamEngine::Core::Render;

class CORE_API MeshFactory
    {
public:
    static Mesh* CreateMesh(Shape shape);

   private:
    static Mesh* GetMesh();
    static void SetCubeMesh(Mesh* mesh);
    static void SetSphereMesh(Mesh* mesh, int latitudeSegments = 16, int longitudeSegments = 32);
    static void SetCapsuleMesh(Mesh* mesh, float radius, float height, int segments);
    static void SetCylinderMesh(Mesh* mesh, int segments = 32, float radius = 0.5f, float height = 1.0f);
    static void SetPlaneMesh(Mesh* mesh);
};
}  // namespace DreamEngine::Core::Render
#endif