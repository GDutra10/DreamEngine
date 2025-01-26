#ifndef PROFILING_PROFILER_RESULT_H_
#define PROFILING_PROFILER_RESULT_H_
#include <string>
#include <sstream>
#include "../Core/Serializers/IJsonSerializable.h"

namespace DreamEngine::Profiling
{
struct ProfilerResult : Core::IJsonSerializable<ProfilerResult>
{
public:
	std::string name = "Default";
	long long start = 0;
	long long end = 0;
	size_t threadId = 0;

    std::string SerializeToJson() override
    {
        std::ostringstream oss;

        oss << "{";
        oss << R"("ph":"X",)";
        oss << R"("cat":"function",)";
        oss << R"("pid":)" + std::to_string(0) + ",";
        oss << R"("tid":)" + std::to_string(threadId) + ",";
        oss << R"("name":")" + name + "\",";
        oss << R"("ts":)" + std::to_string(start) + ",";
        oss << R"("dur":)" + std::to_string(end - start);
        oss << "}";

        return oss.str();
    }
};
}
#endif