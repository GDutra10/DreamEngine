#ifndef EDITOR_CONTROLLERS_SCRIPT_CONTROLLER_H_
#define EDITOR_CONTROLLERS_SCRIPT_CONTROLLER_H_

namespace DreamEngine::Editor::Controllers
{
class ScriptController
{
   public:
    void ReloadScripts();
   private:
    bool BuildSolution();
};

}  // namespace DreamEngine::Editor::Controllers
#endif