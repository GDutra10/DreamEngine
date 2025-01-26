#include "ProfilerTimer.h"

#include <thread>

#include "Profiler.h"

using namespace DreamEngine::Profiling;

ProfilerTimer::ProfilerTimer(const std::string& name)
{
	m_result.name = name;

    Start();

    static long long lastStartTime = 0;

    // if it's the same as the previous start time, add 1 to it
    m_result.start += (m_result.start == lastStartTime ? 1 : 0);

    // record the fixed time as the previous start time
    lastStartTime = m_result.start;
}

ProfilerTimer::~ProfilerTimer()
{
	Stop();
}

void ProfilerTimer::AfterStop()
{
    m_result.start = start;
    m_result.end = end;
    m_result.threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());

    Profiler::Instance().AddProfileResult(m_result);
}

