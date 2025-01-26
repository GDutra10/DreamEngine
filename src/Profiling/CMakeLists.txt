# CMakeList.txt : CMake project for Profiling, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (Profiling STATIC
	"Profiler.cpp"
	"Profiler.h"
	"ProfilerResult.h"
	"ProfilerTimer.cpp"
	"ProfilerTimer.h"
)

# add link libraries
target_link_libraries(Profiling PRIVATE Core)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Profiling PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
