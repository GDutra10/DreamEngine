# CMakeList.txt : CMake project for Profiling, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.10)
project(NativeGame)

# Add source to this project's executable.
add_executable (NativeGame
	"main.cpp"
    "FirstScene.h"
    "FirstScene.cpp"

    "Scripts/BoxScript.h"
    "Scripts/BoxScript.cpp"

    "Vendors/stb_image.cpp"
    "Vendors/stb_image.h"
 "Scripts/HudScript.h" "Scripts/HudScript.cpp" "Scripts/CameraScript.h" "Scripts/CameraScript.cpp")

# add link libraries
target_link_libraries(NativeGame 
    PUBLIC glm::glm
    PRIVATE Profiling Core glfw glad)

target_include_directories(NativeGame PRIVATE ${CMAKE_SOURCE_DIR}/../Core)
target_include_directories(NativeGame PRIVATE ${CMAKE_SOURCE_DIR}/src/Vendors/glfw/include)
target_include_directories(Core PRIVATE ${GLM_DIR})

# Add fonts directory to the build process
add_custom_command(
    TARGET NativeGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/Games/NativeGame/Assets $<TARGET_FILE_DIR:NativeGame>/Assets
)
add_custom_command(TARGET NativeGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Core>
    $<TARGET_FILE_DIR:NativeGame>
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NativeGame PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
