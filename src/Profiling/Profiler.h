#ifndef PROFILING_PROFILER_H_
#define PROFILING_PROFILER_H_

#include "ProfilerResult.h"
#include "ProfilerTimer.h"
#include <vector>

#define PROFILING 1

#ifdef PROFILING
  #define TOKEN_PASTE(x, y) x##y
  #define TOKEN_PASTE2(x, y) TOKEN_PASTE(x, y)
  #define PROFILE_SCOPE(name) auto TOKEN_PASTE2(timer, __LINE__) = DreamEngine::Profiling::ProfilerTimer(name);
  #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
#else
  #define PROFILE_SCOPE(name)
  #define PROFILE_FUNCTION()
#endif

namespace DreamEngine::Profiling
{
	class Profiler 
	{
        std::vector<ProfilerResult> m_profilerResults;
        Profiler() = default;
	public:
        ~Profiler() = default;
		static Profiler& Instance();
		void AddProfileResult(const ProfilerResult& profileResult);
        void SaveFile() const;
        void ResetResult();
	};
}

#endif

