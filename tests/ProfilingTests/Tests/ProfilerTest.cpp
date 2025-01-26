#include <fstream>
#include <gtest/gtest.h>
#include "../../../src/Profiling/Profiler.h"

namespace DreamEngine::ProfilingTests
{
	TEST(ProfilerSaveFile_EmptyProfileResult, ShouldReturnJson)
	{
		// arrange
		Profiling::Profiler::Instance().SaveFile();
		std::string temp;
		auto stream = std::ifstream("profiler.json");

		stream >> temp;

		// act & asserts
		EXPECT_EQ(temp, "{\"traceEvents\":[]}");
	}

	TEST(ProfilerSaveFile_WithProfileResult, ShouldReturnJson)
	{
		// arrange
		Profiling::Profiler::Instance().AddProfileResult(Profiling::ProfilerResult());
		Profiling::Profiler::Instance().AddProfileResult(Profiling::ProfilerResult());
		Profiling::Profiler::Instance().SaveFile();
		std::string temp;
		auto stream = std::ifstream("profiler.json");

		stream >> temp;

		// act && asserts
		EXPECT_EQ(temp, R"({"traceEvents":[{"pid":0,"tid":0,"name":"Default","ts":0,"dur":0},{"pid":0,"tid":0,"name":"Default","ts":0,"dur":0}]})");
	}
}