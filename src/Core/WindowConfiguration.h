#ifndef CORE_WINDOW_CONFIGURATION_H_
#define CORE_WINDOW_CONFIGURATION_H_

#include "CoreExport.h"
#include <string>

namespace DreamEngine::Core
{

enum class CORE_API WindowMode
{
    Windowed,
    Borderless,
    Fullscreen
};

struct CORE_API WindowConfiguration
{
    int width = 1280;
    int height = 720;
    std::string title = "DreamEngine";
    WindowMode mode = WindowMode::Windowed;
};
}  // namespace DreamEngine::Core
#endif