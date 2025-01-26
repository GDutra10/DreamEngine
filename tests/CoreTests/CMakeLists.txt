# CMakeList.txt : CMake project for CoreTests, include source and define
# project specific logic here.
#

enable_testing()

# Add source to this project's executable.
add_executable (CoreTests
	"main.cpp"
	"Text/JsonTests.cpp" "Serializers/JsonSerializableTest.cpp" "Math/Vector3Test.cpp" "Math/Vector2Test.cpp" "ECS/ECSTest.cpp")

# add link libraries
target_link_libraries(CoreTests Core)
# Link the test executable against Google Test and your library
target_link_libraries(CoreTests gtest gtest_main Core)

# Register the test
include(GoogleTest)
gtest_discover_tests(CoreTests)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CoreTests PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
