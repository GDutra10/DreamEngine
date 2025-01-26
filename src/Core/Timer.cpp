#include <chrono>
#include "Timer.h"

using namespace DreamEngine::Core;

void Timer::Start()
{
    stopped = false;
    m_startTimePoint = std::chrono::high_resolution_clock::now();
    start = time_point_cast<std::chrono::microseconds>(m_startTimePoint)
              .time_since_epoch()
              .count();
}

long long Timer::Elapsed()
{
	using namespace std::chrono;

    if (!stopped)
        m_stopTimePoint = high_resolution_clock::now();

    end = time_point_cast<microseconds>(m_stopTimePoint)
                           .time_since_epoch()
                           .count();

	return end - start;
}

void Timer::Stop()
{
    using namespace std::chrono;

    if (stopped)
        return;

    stopped = true;

    m_stopTimePoint = high_resolution_clock::now();
    end = time_point_cast<microseconds>(m_stopTimePoint)
              .time_since_epoch()
              .count();

    AfterStop();
}
