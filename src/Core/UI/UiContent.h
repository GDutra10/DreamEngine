#ifndef CORE_UI_UI_CONTENT_H_
#define CORE_UI_UI_CONTENT_H_

#include "../Resources/Resource.h"

namespace DreamEngine::Core::UI
{
using namespace DreamEngine::Core::Resources;

class CORE_API UiContent : public Resource
{
public:
    std::string text;
};
}  // namespace DreamEngine::Core::Render
#endif