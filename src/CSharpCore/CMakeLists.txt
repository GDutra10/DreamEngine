cmake_minimum_required(VERSION 3.12)

# Set the project name
project(DreamEngine LANGUAGES CSharp)

# Find the .csproj file
set(CSPROJ_FILE "${CMAKE_SOURCE_DIR}/src/CSharpCore/DreamEngine/DreamEngine.csproj")

# Add a custom target to build the .csproj file using MSBuild
add_custom_target(DreamEngine ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Building C# project..."
    COMMAND MSBuild.exe ${CSPROJ_FILE} /p:Configuration=Release
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/CSharpCore/DreamEngine
    COMMENT "Building C# project using MSBuild"
)

# Set the output directory for the build
set_target_properties(DreamEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)