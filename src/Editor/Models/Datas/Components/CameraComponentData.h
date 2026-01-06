#ifndef EDITOR_MODELS_DATAS_COMPONENTS_CAMERA_COMPONENT_DATA_H_
#define EDITOR_MODELS_DATAS_COMPONENTS_CAMERA_COMPONENT_DATA_H_

namespace DreamEngine::Editor::Models::Datas::Components
{
struct CameraComponentData
{
    float fovDegree = 45.0f;
    float near = 0.1f;
    float far = 100.0f;
    bool has = false;
};

}  // namespace DreamEngine::Editor::Models::Datas::Components
#endif