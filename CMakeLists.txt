# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("DreamEngine")

# set(CMAKE_CSharp_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/csc.exe")

# glad configuration
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/Vendors/glad/include")
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/src/Vendors/glad/src/glad.c")
include_directories(${GLAD_INCLUDE_DIR})
add_library(glad STATIC ${GLAD_SOURCE})
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

# Include sub-projects.
add_subdirectory("src/Core")
add_subdirectory("src/Profiling")
add_subdirectory("src/Editor")
# add_subdirectory("src/CSharpCore")
add_subdirectory("src/Games/NativeGame")
add_subdirectory("src/Vendors/glfw")

set(GLM_DIR "src/Vendors/glm")
add_subdirectory(${GLM_DIR})

set(ASSIMP_DIR "src/Vendors/assimp")
add_subdirectory(${ASSIMP_DIR})

set(JSON_DIR "src/Vendors/json")
add_subdirectory(${JSON_DIR})

# Include Google Test
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)


# add tests
add_subdirectory("tests/CoreTests")
add_subdirectory("tests/ProfilingTests")
