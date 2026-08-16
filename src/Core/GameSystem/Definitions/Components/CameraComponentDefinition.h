#ifndef CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_CAMERA_COMPONENT_DEINITION_H_
#define CORE_GAME_SYSTEM_DEFINITIONS_COMPONENTS_CAMERA_COMPONENT_DEINITION_H_

namespace DreamEngine::Core::GameSystem::Definitions::Components
{
struct CameraComponentDefinition
{
    float fovDegree = 45.0f;
    float near = 0.1f;
    float far = 100.0f;
    bool has = false;
};

}  // namespace DreamEngine::Core::GameSystem::Definitions::Components
#endif