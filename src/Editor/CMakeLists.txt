# CMakeList.txt : CMake project for Editor, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.10)
project(Editor)

# Add source to this project's executable.
add_executable (Editor
	"Editor.cpp"
	"Editor.h"
	"EditorDefine.h"
	"EditorScene.h"
	"EditorScene.cpp"
	"Controllers/CameraEditorController.h"
	"Controllers/CameraEditorController.cpp"
	"Controllers/EntityController.h"
	"Controllers/EntityController.cpp"
	"Controllers/SceneController.h"
	"Controllers/SceneController.cpp"
	"Controllers/ResourceController.h"
	"Controllers/ResourceController.cpp"
	"Controllers/ScriptController.h"
	"Controllers/ScriptController.cpp"
	"Helpers/FileHelper.h"
	"Helpers/FileHelper.cpp"
	"Helpers/ImGuiHelper.h"
	"Helpers/ImGuiHelper.cpp"
	"Helpers/TextureHelper.h"
	"Helpers/TextureHelper.cpp"

	"Importers/BaseModelImporter.h"
	"Importers/AssimpModelImporter.h"
	"Importers/AssimpModelImporter.cpp"

	"Loggers/EditorLogger.h"
	"Loggers/EditorLogger.cpp"
	
	"Models/EditorConfiguration.h"
	"Models/EditorConfiguration.cpp"
	"Models/EntityConfig.h"	
	"Models/ProjectConfiguration.h"
	"Models/ProjectConfiguration.cpp"
	"Models/Result.h"
	"Models/SceneData.h"
	"Models/SceneConfig.h"
	"Models/Model.h"
	"Models/FileIcon.h"

	"Serializers/MaterialSerializer.h"
	"Serializers/MaterialSerializer.cpp"
	"Serializers/Vec3Serializer.h"
	"Serializers/Vec3Serializer.cpp" 
	"Serializers/ModelSerializer.h"
	"Serializers/ModelSerializer.cpp"
	"Serializers/TextureSerializer.h"
	"Serializers/TextureSerializer.cpp"

	"Singletons/EditorSingleton.h"
	"Singletons/EditorSingleton.cpp"
	
	"UI/Modals/BaseModal.h"
	"UI/Modals/BaseModal.cpp"
	"UI/Modals/ImportResourceModal.h"
	"UI/Modals/ImportResourceModal.cpp"
	"UI/Modals/GetNameModal.h"
	"UI/Modals/GetNameModal.cpp"
	"UI/Views/ColorView.h"
	"UI/Views/ColorView.cpp"
	"UI/Views/LogView.h"
	"UI/Views/LogView.cpp"
	"UI/Views/MaterialView.h"
	"UI/Views/MaterialView.cpp"
	"UI/Windows/BaseWindow.h"
	"UI/Windows/HierarchyWindow.h"
	"UI/Windows/PropertyWindow.h"
	"UI/Windows/SceneWindow.h"
	"UI/Windows/LoggerWindow.h"
	"UI/Windows/HierarchyWindow.cpp"
	"UI/Windows/LoggerWindow.cpp"
	"UI/Windows/PropertyWindow.cpp"
	"UI/Windows/SceneWindow.cpp"
	"UI/Windows/BaseWindow.cpp"
	"UI/Windows/ProjectWindow.h"
	"UI/Windows/ProjectWindow.cpp"
	"UI/Windows/FileExplorerWindow.h"
	"UI/Windows/FileExplorerWindow.cpp"
	"UI/Windows/ResourceManagerWindow.h"
	"UI/Windows/ResourceManagerWindow.cpp"
	"UI/Windows/MaterialWindow.h" 
	"UI/Windows/MaterialWindow.cpp"
	"UI/Modals/FileDialogModal.h" 
	"UI/Modals/FileDialogModal.cpp" 

	"Vendors/stb_image.h"
	"Vendors/stb_image.cpp"
	"Vendors/imgui/imconfig.h"	
	"Vendors/imgui/imgui.cpp"
	"Vendors/imgui/imgui.h"
	"Vendors/imgui/imgui_demo.cpp"
	"Vendors/imgui/imgui_draw.cpp"
	"Vendors/imgui/imgui_internal.h"
	"Vendors/imgui/imgui_tables.cpp"
	"Vendors/imgui/imgui_widgets.cpp"
	"Vendors/imgui/imstb_rectpack.h"
	"Vendors/imgui/imstb_textedit.h"
	"Vendors/imgui/imstb_truetype.h"
	"Vendors/imgui/GraphEditor.cpp"
	"Vendors/imgui/GraphEditor.h"
	"Vendors/imgui/ImCurveEdit.cpp"
	"Vendors/imgui/ImCurveEdit.h"
	"Vendors/imgui/ImGradient.cpp"
	"Vendors/imgui/ImGradient.h"
	"Vendors/imgui/ImGuizmo.cpp"
	"Vendors/imgui/ImGuizmo.h"
	"Vendors/imgui/ImSequencer.cpp"
	"Vendors/imgui/ImSequencer.h"
	"Vendors/imgui/ImZoomSlider.h"
	"Vendors/imgui/imgui_impl_opengl3.cpp"
	"Vendors/imgui/imgui_impl_opengl3.h"
	"Vendors/imgui/imgui_impl_opengl3_loader.h"
	"Vendors/imgui/imgui_impl_glfw.cpp"
	"Vendors/imgui/imgui_impl_glfw.h"
    )

# add link libraries
target_link_libraries(Editor PRIVATE Profiling Core glm glfw glad assimp nlohmann_json)

target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/../Core)
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/src/Vendors/glfw/include)
target_include_directories(Core PRIVATE ${GLM_DIR})
target_include_directories(Editor PRIVATE ${ASSIMP_DIR})
target_include_directories(Editor PRIVATE ${JSON_DIR})

# Add fonts directory to the build process
add_custom_command(
    TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/Editor/Assets $<TARGET_FILE_DIR:Editor>/Assets
)
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Core>
    $<TARGET_FILE_DIR:Editor>
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Editor PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
