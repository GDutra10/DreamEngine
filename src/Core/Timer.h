#ifndef CORE_TIMER_H_
#define CORE_TIMER_H_

#include <chrono>

namespace DreamEngine::Core
{
class Timer 
{
public:
    void Start();
    long long Elapsed();
    void Stop();
    virtual ~Timer() = default;
protected:
	long long start = 0, end = 0;
    bool stopped = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_stopTimePoint;
    virtual void AfterStop() {}
};
}
#endif