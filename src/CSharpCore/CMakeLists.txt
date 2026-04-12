cmake_minimum_required(VERSION 3.12)
project(CSharpCore)

# Path to the .csproj file
set(CSPROJ_FILE "${CMAKE_SOURCE_DIR}/src/CSharpCore/DreamEngine/DreamEngine.csproj")

# Find dotnet CLI
find_program(DOTNET_PATH NAMES dotnet)
if(NOT DOTNET_PATH)
    message(FATAL_ERROR "dotnet CLI not found. Please install the .NET SDK.")
endif()

# Build the C# project
add_custom_target(CSharpCore ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Building C# project with dotnet..."
    COMMAND ${DOTNET_PATH} build "${CSPROJ_FILE}" -c $<CONFIG> -v minimal
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/CSharpCore/DreamEngine"
    COMMENT "Building C# project (${CSPROJ_FILE})"
)