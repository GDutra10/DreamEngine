#include <fstream>
#include "Profiler.h"

using namespace DreamEngine::Profiling;

Profiler& Profiler::Instance()
{
    static Profiler instance;

	return instance;
}

void Profiler::AddProfileResult(const ProfilerResult& profileResult)
{
    m_profilerResults.push_back(profileResult);
}

void Profiler::SaveFile() const
{
    std::ostringstream oss;
    oss << "{\"traceEvents\":[";

    for (size_t i = 0; i < m_profilerResults.size(); i++) 
    {
        auto result = m_profilerResults[i];
        oss << result.SerializeToJson();

        if (i != m_profilerResults.size() - 1)
            oss << ",";
    }

    oss << "]}";

    auto out = std::ofstream("profiler.json");
    out << oss.str();
    out.close();
}

void Profiler::ResetResult()
{
    m_profilerResults.clear();
}
