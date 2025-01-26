# CMakeList.txt : CMake project for CoreTests, include source and define
# project specific logic here.
#

enable_testing()

# Add source to this project's executable.
add_executable (ProfilingTests
	"main.cpp"
	"Tests/ProfilerTest.cpp")

# add link libraries
target_link_libraries(ProfilingTests Profiling)
# Link the test executable against Google Test and your library
target_link_libraries(ProfilingTests gtest gtest_main Profiling)

# Register the test
include(GoogleTest)
gtest_discover_tests(ProfilingTests)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ProfilingTests PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
