cmake_minimum_required(VERSION 3.12)
project(CSharpCore)

# Path to the .csproj file
set(CSPROJ_FILE "${CMAKE_SOURCE_DIR}/src/CSharpCore/DreamEngine/DreamEngine.csproj")

# Find dotnet CLI
find_program(DOTNET_PATH NAMES dotnet)
if(NOT DOTNET_PATH)
    message(FATAL_ERROR "dotnet CLI not found. Please install the .NET SDK.")
endif()

# Set build command using dotnet
set(BUILD_COMMAND
    "${DOTNET_PATH}" build "${CSPROJ_FILE}" -c $<CONFIG>
)

# Target to build the C# project
add_custom_target(CSharpCore ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Building C# project with dotnet..."
    COMMAND ${BUILD_COMMAND}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/CSharpCore/DreamEngine"
    COMMENT "Building C# project (${CSPROJ_FILE})"
)

# Copy built binaries to CMake's bin/ directory after build
add_custom_command(TARGET CSharpCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/CSharpCore/DreamEngine/bin/$<CONFIG>"
        "${CMAKE_BINARY_DIR}/bin"
    COMMENT "Copying built files to ${CMAKE_BINARY_DIR}/bin"
)
