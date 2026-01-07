# CMakeList.txt : CMake project for Core, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.10)
project(Core)

# Add source to this project's executable.
add_library (Core SHARED
	"Timer.cpp"
	"Timer.h"
	"Serializers/IJsonSerializable.h"

	"Application.h"
	"Application.cpp"
	
	"Color.h"
	"CoreCApi.h"
	"CoreCApi.cpp"
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
	"ECS/Components/NativeScriptComponent.h"
	"ECS/Components/ScriptComponent.h"
	"ECS/Components/ChildrenComponent.h"
	"ECS/Components/ParentComponent.h"
	"ECS/Components/CameraComponent.h"
	"ECS/Components/OutlineComponent.h"
	"ECS/Components/UiComponent.h"
	
	"GameSystem/Camera.h" 
	"GameSystem/Game.h"
	"GameSystem/Game.cpp"
	"GameSystem/GlobalLight.h"
	 
	"GameSystem/Scene.h"
	"GameSystem/Scene.cpp"

	"Helpers/DateHelper.h"
	"Helpers/DateHelper.cpp"
	"Helpers/GUIDHelper.h"
	"Helpers/GUIDHelper.cpp"
	"Helpers/LoggerHelper.h"
	"Helpers/LoggerHelper.cpp"

	"IO/File.h"
	"IO/File.cpp"
	"Inputs/Input.h" 
	"Inputs/Input.cpp" 
	"Inputs/Key.h" 
	"Inputs/KeyState.h"
	"Inputs/MouseButton.h"

	"Loggers/ConsoleLogger.h" 
	"Loggers/ConsoleLogger.cpp"
	"Loggers/Log.h" 
	"Loggers/LogLevel.h" 
	"Loggers/Logger.h" 
	"Loggers/LoggerSingleton.h"
	"Loggers/LoggerSingleton.cpp"
	"Loggers/LoggerExternal.h"
	"Loggers/LoggerExternal.cpp"

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
	"Render/FrameBuffer.h"
	"Render/OutlineOptions.h"
	"Render/OutlineScope.h"
	"Render/OpenGL/OpenGLFrameBuffer.h"
	"Render/OpenGL/OpenGLFrameBuffer.cpp"
	"Render/OpenGL/OpenGLMesh.h" 
	"Render/OpenGL/OpenGLMesh.cpp" 
	"Render/OpenGL/OpenGLShader.h" 
	"Render/OpenGL/OpenGLRenderAPI.h"
	"Render/OpenGL/OpenGLRenderAPI.cpp"
	"Render/Factories/MeshFactory.h" 
	"Render/Factories/MeshFactory.cpp" 
	"Render/Helpers/RenderTypeHelper.h"
	"Render/RenderView.h" 
	"Render/RenderPipeline.h"
	"Render/RenderPipeline.cpp"
	"Render/RenderViewProvider.h"
	"Render/RenderViewProvider.cpp"

	"Resources/Resource.h"
	"Resources/ResourceManager.h"
	"Resources/ResourceManager.cpp" 
	
	"Scripting/coreclrhost.h"
	"Scripting/NativeScript.h"
	"Scripting/NativeScript.cpp"
	"Scripting/ScriptEngine.h"
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

	 "UI/UiContent.h"
	 "UI/UiManager.h"
	 "UI/UiManager.cpp"
	 "UI/IUiHandler.h"
	 "UI/RmlUI/RmlHandler.h" 
	 "UI/RmlUI/RmlHandler.cpp"   
	 "UI/RmlUI/CustomRenderInterface_GL3.h" 
	 "UI/RmlUI/CustomRenderInterface_GL3.cpp" 
)

	
set_target_properties(Core PROPERTIES
    #WINDOWS_EXPORT_ALL_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
)

# rmlui backend configuration
set(RMLUI_DIR ${CMAKE_SOURCE_DIR}/src/Vendors/RmlUi)
set(RMLUI_BACKENDS_DIR ${RMLUI_DIR}/Backends)

target_sources(Core PRIVATE
  ${RMLUI_BACKENDS_DIR}/RmlUi_Platform_GLFW.cpp
  ${RMLUI_BACKENDS_DIR}/RmlUi_Renderer_GL3.cpp
)

target_include_directories(Core PUBLIC 
	${RMLUI_DIR}/Include
    ${RMLUI_BACKENDS_DIR}
)

target_compile_definitions(Core PRIVATE 
    RMLUI_GL3_CUSTOM_LOADER="glad/glad.h"
)

# add link libraries
target_link_libraries(Core 
	PUBLIC 
		glm::glm
	PRIVATE 
		glfw 
		glad
		RmlUi::Core)

# Include GLFW headers
#target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/src/Vendors/glfw/include)
#target_include_directories(Core PUBLIC ${GLM_DIR})
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the export macro
target_compile_definitions(Core PRIVATE CORE_EXPORTS)

# Enable Edit and Continue for MSVC in Debug mode
if(MSVC)
    target_compile_options(Core PRIVATE
        $<$<CONFIG:Debug>:/ZI>           # Edit and Continue debug info
        $<$<CONFIG:Debug>:/Od>           # Disable optimizations
        $<$<CONFIG:Debug>:/RTC1>         # Runtime checks
    )
    # Also need to set linker flags for Edit and Continue
    target_link_options(Core PRIVATE
        $<$<CONFIG:Debug>:/INCREMENTAL>  # Incremental linking
        $<$<CONFIG:Debug>:/SAFESEH:NO>   # Disable Safe Exception Handlers (required for /ZI)
    )
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Core PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
