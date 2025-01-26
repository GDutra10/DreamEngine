#include "GUIDHelper.h"

#include <windows.h>
#include <rpc.h>
#include <sstream>
#include <iomanip>

#include "../Loggers/LoggerSingleton.h"


using namespace DreamEngine::Core::Helpers;

std::string GUIDHelper::GenerateGUID()
{
    GUID guid;

    if (CoCreateGuid(&guid) != S_OK)
    {
        Loggers::LoggerSingleton::Instance().LogError("Error to generate the guid!");

        return "Error to generate GUID";
    }

    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(8)
        << guid.Data1 << "-" << std::setw(4)
        << guid.Data2 << "-" << std::setw(4)
        << guid.Data3 << "-" << std::setw(2)
        << static_cast<int>(guid.Data4[0]) << std::setw(2)
        << static_cast<int>(guid.Data4[1]) << "-" << std::setw(2)
        << static_cast<int>(guid.Data4[2]) << std::setw(2)
        << static_cast<int>(guid.Data4[3]) << std::setw(2)
        << static_cast<int>(guid.Data4[4]) << std::setw(2)
        << static_cast<int>(guid.Data4[5]) << std::setw(2)
        << static_cast<int>(guid.Data4[6]) << std::setw(2)
        << static_cast<int>(guid.Data4[7]);

    return oss.str();
}
