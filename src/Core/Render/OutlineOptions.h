#ifndef CORE_RENDER_OUTLINE_OPTIONS_H_
#define CORE_RENDER_OUTLINE_OPTIONS_H_

#include "CoreExport.h"

namespace DreamEngine::Core::Render
{
struct CORE_API OutlineOptions
{
    bool disableDepthTest = false;
    enum class CullFace
    {
        Front, 
        Back
    } cullFace = CullFace::Front;
};
}  // namespace DreamEngine::Core::Render
#endif