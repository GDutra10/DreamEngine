#ifndef CORE_IO_FILE_H_
#define CORE_IO_FILE_H_

#include <string>
#include <vector>

#include "CoreExport.h"

namespace DreamEngine::Core::IO
{
class CORE_API File
{
public:
    static std::string ReadAllText(const std::string& filePath);
    static std::vector<uint8_t> ReadBinary(const std::string& filePath);
};
}  // namespace DreamEngine::Core::IO
#endif