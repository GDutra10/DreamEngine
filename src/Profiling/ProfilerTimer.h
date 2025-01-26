#ifndef PROFILING_PROFILER_TIMER_H_
#define PROFILING_PROFILER_TIMER_H_

#include <chrono>

#include "ProfilerResult.h"
#include "../Core/Timer.h"


namespace DreamEngine::Profiling
{
	class ProfilerTimer : public DreamEngine::Core::Timer
	{
	public:
        explicit ProfilerTimer(const std::string& name);
		~ProfilerTimer() override;
	protected:
        void AfterStop() override;
    private:
		ProfilerResult m_result;
	};
}
#endif
