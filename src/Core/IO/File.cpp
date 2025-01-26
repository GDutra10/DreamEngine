#include "File.h"
#include <fstream>
#include <sstream>

#include "../Loggers/LoggerSingleton.h"


using namespace DreamEngine::Core::IO;

std::string File::ReadAllText(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        Loggers::LoggerSingleton::Instance().LogError("Error opening file: " + filePath);
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}
