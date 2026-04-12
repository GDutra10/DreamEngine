
#ifndef CORE_SYNC_SCENE_DATA_H_
#define CORE_SYNC_SCENE_DATA_H_
#include <cstdint>

namespace DreamEngine::Core::Sync
{
struct SceneData
{
    unsigned int mainCameraEntityId;
    uint8_t showCursor;
    uint8_t mustRecreateEntities;
    float globalLightColorR;
    float globalLightColorG;
    float globalLightColorB;
    float globalLightIntensity;
};
}  // namespace DreamEngine::Core::Sync
#endif