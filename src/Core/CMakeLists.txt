# CMakeList.txt : CMake project for Core, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.10)
project(Core)

# Add source to this project's executable.
add_library (Core STATIC
	"Timer.cpp"
	"Timer.h"
	"Serializers/IJsonSerializable.h"

	"Application.h"
	"Application.cpp"
	"Color.h"
	"CoreExport.h"
	"EngineDefine.h"

	"ECS/Entity.h"
	"ECS/Entity.cpp"
	"ECS/EntityManager.h"
	"ECS/EntityManager.cpp"
	"ECS/EntityMemoryPool.h"
	"ECS/EntityMemoryPool.cpp"
	"ECS/Components/Component.h"
	"ECS/Components/TransformComponent.h"
	"ECS/Components/TransformComponent.cpp" 
	"ECS/Components/MeshComponent.h"
	"ECS/Components/DirectionalLightComponent.h"
	"ECS/Components/MaterialComponent.h"
	"ECS/Components/ScriptComponent.h"
	"ECS/Components/ChildrenComponent.h"
	"ECS/Components/ParentComponent.h"
	
	"GameSystem/Camera.h" 
	"GameSystem/Game.h"
	"GameSystem/Game.cpp"
	"GameSystem/GlobalLight.h"
	 
	"GameSystem/Scene.h"
	"GameSystem/Scene.cpp"

	"Helpers/DateHelper.h"
	"Helpers/GUIDHelper.h"
	"Helpers/GUIDHelper.cpp"
	"Helpers/LoggerHelper.h"	

	"IO/File.h"
	"IO/File.cpp"
	"Inputs/Input.h" 
	"Inputs/Input.cpp" 
	"Inputs/Key.h" 
	"Inputs/KeyState.h"
	"Inputs/MouseButton.h"

	"Loggers/Log.h" 
	"Loggers/LogLevel.h" 
	"Loggers/Logger.h" 
	"Loggers/ConsoleLogger.h" 
	"Loggers/LoggerSingleton.h" 

	"Math/Math.h" 

	"Render/RenderAPI.cpp" 
	"Render/Vertex.h"
	"Render/Texture.h"
	"Render/Shader.h"
	"Render/Mesh.h"
	"Render/RenderAPI.h"
	"Render/Material.h"
	"Render/RenderType.h" 
	"Render/TextureType.h" 
	"Render/Shape.h"  
	"Render/OpenGL/OpenGLMesh.h" 
	"Render/OpenGL/OpenGLMesh.cpp" 
	"Render/OpenGL/OpenGLShader.h" 
	"Render/OpenGL/OpenGLRenderAPI.h"
	"Render/OpenGL/OpenGLRenderAPI.cpp"
	"Render/Factories/MeshFactory.h" 
	"Render/Factories/MeshFactory.cpp" 
	"Render/Helpers/RenderTypeHelper.h"  

	"Resources/Resource.h"
	"Resources/ResourceManager.h"
	"Resources/ResourceManager.cpp"
	"Resources/GlobalResourceManager.h" 
	"Resources/GlobalResourceManager.cpp" 
	
	"Scripting/ScriptEngine.h"
	"Scripting/coreclrhost.h"
	"Scripting/ScriptEngine.cpp" 
	"Scripting/Script.h" 
	"Scripting/Script.cpp" 
	"Scripting/ScriptInfo.h"
	
	"Sync/EntitySynchronizer.h" 
	"Sync/EntityData.h" 
	"Sync/EntitySynchronizer.cpp"
	"Sync/InputData.h"
	"Sync/InputSynchronizer.h"
	"Sync/InputSynchronizer.cpp"
	"Sync/GameData.h"
	"Sync/GameSynchronizer.h"
	"Sync/GameSynchronizer.cpp"

	"Vendors/glad.h"
	"Vendors/glfw3.h" 
   "Render/FrameBuffer.h")

# add link libraries
target_link_libraries(Core PRIVATE glm glfw glad)

# Include GLFW headers
target_include_directories(Core PRIVATE ${CMAKE_SOURCE_DIR}/src/Vendors/glfw/include)
target_include_directories(Core PRIVATE ${GLM_DIR})
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the export macro
target_compile_definitions(Core PRIVATE CORE_EXPORTS)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Core PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
