#include "MeshFactory.h"

#include "../RenderType.h"
#include "../OpenGL/OpenGLMesh.h"
#include "../Helpers/RenderTypeHelper.h"
#include "../../Application.h"
#include "../../Loggers/LoggerSingleton.h"
#include "../../Resources/ResourceManager.h"
#include "../../EngineDefine.h"
#include "Math/Math.h"

using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Core::Render::Helpers;
using namespace DreamEngine::Core::Render::Factories;
using namespace DreamEngine::Core::Render::OpenGL;
using namespace DreamEngine::Core::Resources;


Mesh* MeshFactory::CreateMesh(const Shape shape)
{
    Mesh* mesh = GetMesh();
    mesh->textures.push_back(ResourceManager::Instance().GetTexture(DEFAULT_TEXTURE_NAME));

    switch (shape)
    {
        case Cube:
            SetCubeMesh(mesh);
            break;
        case Sphere:
            SetSphereMesh(mesh);
            break;
        case Capsule:
            SetCapsuleMesh(mesh, 0.5f, 2.0f, 32);
            break;
        case Cylinder:
            SetCylinderMesh(mesh);
            break;
        case Plane:
            SetPlaneMesh(mesh);
            break;
        //TODO: other shapes
        default:
            Loggers::LoggerSingleton::Instance().LogError("Shape not found!");
    }

    return mesh;
}

Mesh* MeshFactory::GetMesh()
{
    Mesh* mesh = nullptr;
    const RenderType renderType = Application::Instance().GetRenderType();

    switch (renderType)
    {
        case openGL:
            mesh = new OpenGLMesh({},{},{});
            break;
        case vulkan:
        default:
            Loggers::LoggerSingleton::Instance().LogError(
                "Mesh for render type '" + RenderTypeHelper::ToString(renderType) + "' not found!");
    }

    return mesh;
}

void MeshFactory::SetCubeMesh(Mesh* mesh)
{
    mesh->indices = {
        // Back (-Z)
        0, 2, 1,  0, 3, 2,
        // Front (+Z)
        4, 5, 6,  4, 6, 7,
        // Left (-X)
        8,10, 9,  8,11,10,
        // Right (+X)
        12,13,14, 12,14,15,
        // Bottom (-Y)
        16,17,18, 16,18,19,
        // Top (+Y)
        20,22,21, 20,23,22
    };

    mesh->vertices = {
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},  // Bottom-left-back
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},   // Bottom-right-back
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},    // Top-right-back
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},   // Top-left-back

        // Front face
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // Bottom-left-front
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},   // Bottom-right-front
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},    // Top-right-front
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},   // Top-left-front

        // Left face
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // Bottom-left-back
        {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},   // Top-left-back
        {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},    // Top-left-front
        {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},   // Bottom-left-front

        // Right face
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // Bottom-right-back
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},   // Top-right-back
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},    // Top-right-front
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},   // Bottom-right-front

        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},  // Bottom-left-back
        {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},   // Bottom-right-back
        {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},    // Bottom-right-front
        {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},   // Bottom-left-front

        // Top face
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // Top-left-back
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},   // Top-right-back
        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},    // Top-right-front
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}    // Top-left-front
    };
}

void MeshFactory::SetSphereMesh(Mesh* mesh, const int latitudeSegments, const int longitudeSegments)
{
    for (int lat = 0; lat <= latitudeSegments; ++lat)
    {
        const float theta = lat * M_PI / latitudeSegments;  // Latitude angle
        const float sinTheta = sin(theta);
        const float cosTheta = cos(theta);

        for (int lon = 0; lon <= longitudeSegments; ++lon)
        {
            const float phi = lon * 2 * M_PI / longitudeSegments;  // Longitude angle
            const float sinPhi = sin(phi);
            const float cosPhi = cos(phi);

            glm::vec3 position(sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
            glm::vec3 normal = glm::normalize(position);
            glm::vec2 uv((float)lon / longitudeSegments, (float)lat / latitudeSegments);

            mesh->vertices.push_back({position, normal, uv});
        }
    }

    for (int lat = 0; lat < latitudeSegments; ++lat)
    {
        for (int lon = 0; lon < longitudeSegments; ++lon)
        {
            const int first = lat * (longitudeSegments + 1) + lon;
            const int second = first + longitudeSegments + 1;

            mesh->indices.push_back(first);
            mesh->indices.push_back(second);
            mesh->indices.push_back(first + 1);

            mesh->indices.push_back(second);
            mesh->indices.push_back(second + 1);
            mesh->indices.push_back(first + 1);
        }
    }
}

// FIXME: top hemisphere is not correct
void MeshFactory::SetCapsuleMesh(Mesh* mesh, const float radius, const float height, const int segments)
{
    int latSegments = segments / 2;  // Half of the segments for each hemisphere
    int lonSegments = segments;
    float cylinderHeight = height - (2.0f * radius);  // Exclude the caps
    int vertexCountPerCircle = lonSegments + 1;       // Includes duplicated vertices for texture wrapping

    // === Bottom Hemisphere ===
    for (int lat = 0; lat <= latSegments; ++lat)
    {
        float v = (float)lat / latSegments;  // Vertical interpolation
        float theta = v * (M_PI / 2.0f);     // From bottom pole to equator
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= lonSegments; ++lon)
        {
            float u = (float)lon / lonSegments;  // Horizontal interpolation
            float phi = u * 2.0f * M_PI;         // Around the circumference
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            glm::vec3 position = glm::vec3(radius * sinTheta * cosPhi,
                                           -cylinderHeight / 2.0f - radius * cosTheta,  // Bottom hemisphere
                                           radius * sinTheta * sinPhi);
            glm::vec3 normal = glm::normalize(position - glm::vec3(0.0f, -cylinderHeight / 2.0f, 0.0f));
            glm::vec2 uv(u, v / 2.0f);

            mesh->vertices.push_back({position, normal, uv});
        }
    }

    // === Cylinder Body ===
    for (int i = 0; i <= 1; ++i)
    {                                                           // Two rows: top and bottom of the cylinder
        float y = -cylinderHeight / 2.0f + i * cylinderHeight;  // Y position
        for (int lon = 0; lon <= lonSegments; ++lon)
        {
            float u = (float)lon / lonSegments;  // Horizontal position around circumference
            float phi = u * 2.0f * M_PI;         // Around the circumference
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            glm::vec3 position = glm::vec3(radius * cosPhi, y, radius * sinPhi);
            glm::vec3 normal = glm::normalize(glm::vec3(cosPhi, 0.0f, sinPhi));
            glm::vec2 uv(u, 0.5f + i * 0.5f);  // Map UV to cylinder body

            mesh->vertices.push_back({position, normal, uv});
        }
    }

    // === Top Hemisphere ===
    for (int lat = 0; lat <= latSegments; ++lat)
    {
        float v = (float)lat / latSegments;  // Vertical interpolation
        float theta = v * (M_PI / 2.0f);     // From equator to top pole
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= lonSegments; ++lon)
        {
            float u = (float)lon / lonSegments;  // Horizontal interpolation
            float phi = u * 2.0f * M_PI;         // Around the circumference
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            glm::vec3 position = glm::vec3(radius * sinTheta * cosPhi,
                                           cylinderHeight / 2.0f + radius * (1.0f - cosTheta),  // Top hemisphere
                                           radius * sinTheta * sinPhi);
            glm::vec3 normal = glm::normalize(position - glm::vec3(0.0f, cylinderHeight / 2.0f, 0.0f));
            //glm::vec2 uv(u, 0.5f + v / 2.0f);
            glm::vec2 uv(u, v / 2.0f);

            mesh->vertices.push_back({position, normal, uv});
        }
    }

    // === Generate Indices ===
    int offset = 0;

    // Bottom Hemisphere
    for (int lat = 0; lat < latSegments; ++lat)
    {
        for (int lon = 0; lon < lonSegments; ++lon)
        {
            int current = offset + lat * vertexCountPerCircle + lon;
            int next = current + vertexCountPerCircle;

            mesh->indices.push_back(current);
            mesh->indices.push_back(next);
            mesh->indices.push_back(current + 1);

            mesh->indices.push_back(next);
            mesh->indices.push_back(next + 1);
            mesh->indices.push_back(current + 1);
        }
    }
    offset += (latSegments + 1) * vertexCountPerCircle;

    // Cylinder Body
    for (int lon = 0; lon < lonSegments; ++lon)
    {
        int current = offset + lon;
        int next = current + vertexCountPerCircle;

        mesh->indices.push_back(current);
        mesh->indices.push_back(next);
        mesh->indices.push_back(current + 1);

        mesh->indices.push_back(next);
        mesh->indices.push_back(next + 1);
        mesh->indices.push_back(current + 1);
    }
    offset += 2 * vertexCountPerCircle;

    // Top Hemisphere
    for (int lat = 0; lat < latSegments; ++lat)
    {
        for (int lon = 0; lon < lonSegments; ++lon)
        {
            int current = offset + lat * vertexCountPerCircle + lon;
            int next = current + vertexCountPerCircle;

            mesh->indices.push_back(current);
            mesh->indices.push_back(next);
            mesh->indices.push_back(current + 1);

            mesh->indices.push_back(next);
            mesh->indices.push_back(next + 1);
            mesh->indices.push_back(current + 1);
        }
    }
}

// FIXME: 
void MeshFactory::SetCylinderMesh(Mesh* mesh, const int segments, const float radius, const float height)
{
    float halfHeight = height / 2.0f;
    float angleStep = (2.0f * M_PI) / segments;

    // Generate vertices and indices for the top and bottom circles
    int baseIndex = 0;

    // Top center vertex
    mesh->vertices.push_back({{0.0f, halfHeight, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.5f}});
    baseIndex++;

    // Top circle
    for (int i = 0; i < segments; i++)
    {
        float x = radius * cos(i * angleStep);
        float z = radius * sin(i * angleStep);
        float u = (x / radius + 1.0f) / 2.0f;
        float v = (z / radius + 1.0f) / 2.0f;

        mesh->vertices.push_back({{x, halfHeight, z}, {0.0f, 1.0f, 0.0f}, {u, v}});
    }

    // Bottom center vertex
    mesh->vertices.push_back({{0.0f, -halfHeight, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.5f, 0.5f}});
    baseIndex++;

    // Bottom circle
    for (int i = 0; i < segments; i++)
    {
        float x = radius * cos(i * angleStep);
        float z = radius * sin(i * angleStep);
        float u = (x / radius + 1.0f) / 2.0f;
        float v = (z / radius + 1.0f) / 2.0f;

        mesh->vertices.push_back({{x, -halfHeight, z}, {0.0f, -1.0f, 0.0f}, {u, v}});
    }

    // Indices for top cap
    for (int i = 1; i <= segments; i++)
    {
        int next = (i % segments) + 1;
        mesh->indices.push_back(0);     // Center vertex
        mesh->indices.push_back(i);     // Current segment vertex
        mesh->indices.push_back(next);  // Next segment vertex
    }

    // Indices for bottom cap
    int bottomCenterIndex = segments + 1;
    int bottomStartIndex = segments + 2;
    for (int i = 0; i < segments; i++)
    {
        int current = bottomStartIndex + i;
        int next = bottomStartIndex + ((i + 1) % segments);
        mesh->indices.push_back(bottomCenterIndex);  // Center vertex
        mesh->indices.push_back(next);               // Next segment vertex
        mesh->indices.push_back(current);            // Current segment vertex
    }

    // Side vertices and indices
    for (int i = 0; i < segments; i++)
    {
        int currentTop = 1 + i;
        int nextTop = 1 + ((i + 1) % segments);
        int currentBottom = bottomStartIndex + i;
        int nextBottom = bottomStartIndex + ((i + 1) % segments);

        // First triangle (current top, next top, current bottom)
        mesh->indices.push_back(currentTop);
        mesh->indices.push_back(nextTop);
        mesh->indices.push_back(currentBottom);

        // Second triangle (current bottom, next top, next bottom)
        mesh->indices.push_back(currentBottom);
        mesh->indices.push_back(nextTop);
        mesh->indices.push_back(nextBottom);
    }
}

void MeshFactory::SetPlaneMesh(Mesh* mesh)
{
    mesh->indices = {
        0, 1, 2, 2, 3, 0  // Two triangles making the plane (quad)
    };

    mesh->vertices = {
        // Bottom-left
        {{-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // Position, normal, UV
        // Bottom-right
        {{0.5f, 0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // Position, normal, UV
        // Top-right
        {{0.5f, 0.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // Position, normal, UV
        // Top-left
        {{-0.5f, 0.0f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}  // Position, normal, UV
    };
}