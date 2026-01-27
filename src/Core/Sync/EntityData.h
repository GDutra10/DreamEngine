#ifndef CORE_SYNC_ENTITY_DATA_H_
#define CORE_SYNC_ENTITY_DATA_H_

#include <cstdint>

namespace DreamEngine::Core::Sync
{
struct EntityData
{
    unsigned int id;
    // components
    float transformPositionX;
    float transformPositionY;
    float transformPositionZ;
    float transformRotationX;
    float transformRotationY;
    float transformRotationZ;
    float transformScaleX;
    float transformScaleY;
    float transformScaleZ;
    float cameraFov;
    float cameraNear;
    float cameraFar;
    float directionalLightSpecularX;
    float directionalLightSpecularY;
    float directionalLightSpecularZ;
    float directionalLightColorRed;
    float directionalLightColorGreen;
    float directionalLightColorBlue;
    float directionalLightColorAlpha;
    float directionalLightInfluence;

    uint8_t uiComponentHas;
    uint8_t cameraComponentHas;
    uint8_t directionalLightComponentHas;

    char scriptAssemblyName[256];
    char scriptClassName[256];
};
}  // namespace DreamEngine::Core::Sync
#endif