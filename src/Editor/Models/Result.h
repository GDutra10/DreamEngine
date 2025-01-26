#ifndef EDITOR_MODELS_RESULT_H_
#define EDITOR_MODELS_RESULT_H_

#include <string>

namespace DreamEngine::Editor::Models
{
struct Result
{
    std::string errorMessage;
    bool isOk;
};
}  // namespace DreamEngine::Editor::Models
#endif