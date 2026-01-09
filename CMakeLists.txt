# CMakeLists.txt - Top-level build script for DreamEngine

cmake_minimum_required(VERSION 3.12)
project(DreamEngine LANGUAGES C CXX)

# Enable MSVC Hot Reload support if available
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
  )
endif()

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Stop adding a 'd' suffix for Debug (and others) everywhere
set(CMAKE_DEBUG_POSTFIX "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "")
set(CMAKE_MINSIZEREL_POSTFIX "")

# Set global output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

foreach(OUTPUTCONFIG IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin/${OUTPUTCONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib/${OUTPUTCONFIG})
endforeach()

# ---- glad (OpenGL loader) ----
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/Vendors/glad/include")
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/src/Vendors/glad/src/glad.c")
add_library(glad STATIC ${GLAD_SOURCE})
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

# ---- force GLFW to be built as a shared library ----
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

# ---- Sub-projects ----
add_subdirectory(src/Core)
add_subdirectory(src/Profiling)
add_subdirectory(src/Editor)
add_subdirectory(src/Games/NativeGame)
#add_subdirectory(src/Vendors/glfw)

# ---- C#/.NET Core integration ----
add_subdirectory(src/CSharpCore) # This should contain a custom CMakeLists.txt to build your .csproj

# ---- External Libraries ----
set(GLM_DIR "src/Vendors/glm")
#add_subdirectory(${GLM_DIR})
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${GLM_DIR}")
add_library(glm::glm ALIAS glm)

set(GLFW_DIR "src/Vendors/glfw")
add_subdirectory(${GLFW_DIR})

set(ASSIMP_DIR "src/Vendors/assimp")
add_subdirectory(${ASSIMP_DIR})

set(JSON_DIR "src/Vendors/json")
add_subdirectory(${JSON_DIR})

# --- RmlUi options (set BEFORE add_subdirectory) ---
set(RMLUI_SAMPLES OFF CACHE BOOL "" FORCE)

# We compile the backend .cpp files ourselves inside Core.
set(RMLUI_BACKEND "GLFW_GL3" CACHE STRING "" FORCE)

# If you want to build their Backends/ helper target(s), set a backend.
# (Optional if you roll your own RenderInterface)
set(RMLUI_BACKEND GLFW_GL3 CACHE STRING "" FORCE)

# Build FreeType static to avoid runtime DLL dependency
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)

add_subdirectory(src/Vendors/freetype)
add_library(Freetype::Freetype ALIAS freetype)

# Provide OpenGL::GL if your project doesn't use find_package(OpenGL)
# (This is a known gotcha in some setups.) :contentReference[oaicite:1]{index=1}
#add_library(opengl_gl INTERFACE)
#add_library(OpenGL::GL ALIAS opengl_gl)

# --- RmlUi itself ---
set(RMLUI_DIR "src/Vendors/RmlUi")
add_subdirectory(${RMLUI_DIR})

# ---- GoogleTest via FetchContent ----
#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/release-1.10.0.zip
#)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

# ---- Tests ----
add_subdirectory(tests/CoreTests)
add_subdirectory(tests/ProfilingTests)
