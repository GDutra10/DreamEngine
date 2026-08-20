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

std::vector<uint8_t> File::ReadBinary(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    if (!file.is_open())
        return {};

    const std::streamsize size = file.tellg();

    if (size <= 0)
        return {};

    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(static_cast<size_t>(size));

    if (!file.read(reinterpret_cast<char*>(data.data()), size))
    {
        return {};
    }

    return data;
}
