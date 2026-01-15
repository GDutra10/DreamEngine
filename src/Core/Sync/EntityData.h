#ifndef CORE_SYNC_ENTITY_DATA_H_
#define CORE_SYNC_ENTITY_DATA_H_

#include <cstdint>

namespace DreamEngine::Core::Sync
{
struct EntityData
{
    unsigned int id;
    float transformPositionX;
    float transformPositionY;
    float transformPositionZ;
    float transformRotationX;
    float transformRotationY;
    float transformRotationZ;
    float transformScaleX;
    float transformScaleY;
    float transformScaleZ;
    uint8_t uiComponentHas;
    char scriptAssemblyName[256];
    char scriptClassName[256];
};
}  // namespace DreamEngine::Core::Sync
#endif