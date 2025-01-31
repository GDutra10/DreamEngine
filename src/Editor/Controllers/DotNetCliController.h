#ifndef EDITOR_CONTROLLERS_DOT_NET_CLI_CONTROLLER_H_
#define EDITOR_CONTROLLERS_DOT_NET_CLI_CONTROLLER_H_

#include <filesystem>

namespace DreamEngine::Editor::Controllers
{
class DotNetCliController
{
   public:
    DotNetCliController() = default;
    static int Build(const std::string& projectPath, const std::string& solutionName);
    static int NewClassLib(const std::string& projectPath, const std::string& projectName);
    static int NewSolution(const std::string& solutionPath, const std::string& solutionName);
    static int AddProjectToSolution(const std::string& solutionPath, const std::string& solutionName, const std::string& projectName);
};

}  // namespace DreamEngine::Editor::Controllers
#endif