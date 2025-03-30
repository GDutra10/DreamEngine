#ifndef EDITOR_MODELS_FILE_DIALOG_CONFIG_H_
#define EDITOR_MODELS_FILE_DIALOG_CONFIG_H_

namespace DreamEngine::Editor::Models
{
struct FileDialogConfig
{
    bool shouldUseCurrentPath = true;
    bool shouldRenderOnlyTheCurrentPath = true;
    bool shouldRenderOnlyFolder = false;
    bool shouldRenderByExtension = false;
    std::string extension;
};
}  // namespace DreamEngine::Editor::Models
#endif