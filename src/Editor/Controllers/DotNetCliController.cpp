#include "DotNetCliController.h"

using namespace DreamEngine::Editor::Controllers;

int DotNetCliController::Build(const std::string& projectPath, const std::string& solutionName)
{
    std::string command = "dotnet build " + projectPath + "\\" + solutionName;

    return std::system(command.c_str());
}

int DotNetCliController::NewClassLib(const std::string& projectPath, const std::string& projectName)
{
    std::string command = "dotnet new classlib --name " + projectName + " --output " + projectPath;

    const int result = std::system(command.c_str());

    return result;
    
}

int DotNetCliController::NewSolution(const std::string& solutionPath, const std::string& solutionName)
{
    std::string command = "dotnet new sln --name " + solutionName + " --output " + solutionPath;

    return std::system(command.c_str());
}

int DotNetCliController::AddProjectToSolution(const std::string& solutionPath, const std::string& solutionName, const std::string& projectName)
{
    std::string command = "dotnet sln " + solutionPath + "\\" + solutionName + ".sln add " + solutionPath + "\\" + projectName + ".csproj";

    return std::system(command.c_str());
}
