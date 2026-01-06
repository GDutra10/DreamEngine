# CMakeLists.txt - Editor module build script

cmake_minimum_required(VERSION 3.10)
project(Editor)

# Add source files
add_executable(Editor
    Editor.cpp
    Editor.h
    EditorDefine.h
    EditorScene.h
    EditorScene.cpp

    Controllers/CameraEditorController.h
    Controllers/CameraEditorController.cpp
    Controllers/EntityController.h
    Controllers/EntityController.cpp
    Controllers/SceneController.h
    Controllers/SceneController.cpp
    Controllers/ResourceController.h
    Controllers/ResourceController.cpp
    Controllers/ScriptController.h
    Controllers/ScriptController.cpp
    Controllers/ProjectController.h
    Controllers/ProjectController.cpp
    Controllers/DotNetCliController.h
    Controllers/DotNetCliController.cpp

    Helpers/FileHelper.h
    Helpers/FileHelper.cpp
    Helpers/ImGuiHelper.h
    Helpers/ImGuiHelper.cpp
    Helpers/TextureHelper.h
    Helpers/TextureHelper.cpp

    Importers/BaseModelImporter.h
    Importers/AssimpModelImporter.h
    Importers/AssimpModelImporter.cpp

    Loggers/EditorLogger.h
    Loggers/EditorLogger.cpp

    Models/EditorConfiguration.h
    Models/EditorConfiguration.cpp
    Models/ProjectConfiguration.h
    Models/Result.h
    Models/Model.h
    Models/FileIcon.h
    Models/FileDialogConfig.h
    Models/Datas/ColorData.h
    Models/Datas/ComponentsData.h
    Models/Datas/Components/CameraComponentData.h
    Models/Datas/DirectionalLightData.h
    Models/Datas/GlobalLightData.h
    Models/Datas/SceneData.h
    Models/Datas/TransformData.h
    Models/Datas/Vector3Data.h
    Models/Datas/Components/MeshComponentData.h
    Models/Datas/Components/ScriptComponentData.h
    Models/Datas/Components/ParentComponentData.h
    Models/Datas/Components/ChildrenComponentData.h
    Models/Datas/Components/MaterialComponentData.h

    Serializers/MaterialSerializer.h
    Serializers/MaterialSerializer.cpp
    Serializers/Vec3Serializer.h
    Serializers/Vec3Serializer.cpp
    Serializers/ModelSerializer.h
    Serializers/ModelSerializer.cpp
    Serializers/TextureSerializer.h
    Serializers/TextureSerializer.cpp
    Serializers/ProjectConfigurationSerializer.h
    Serializers/ProjectConfigurationSerializer.cpp
    Serializers/SceneDataSerializer.h
    Serializers/SceneDataSerializer.cpp
    Serializers/ShaderSerializer.h

    Singletons/EditorSingleton.h
    Singletons/EditorSingleton.cpp

    UI/Modals/BaseModal.h
    UI/Modals/BaseModal.cpp
    UI/Modals/ImportResourceModal.h
    UI/Modals/ImportResourceModal.cpp
    UI/Modals/GetNameModal.h
    UI/Modals/GetNameModal.cpp
    UI/Modals/OpenProjectModal.h
    UI/Modals/OpenProjectModal.cpp
    UI/Modals/FileDialogModal.h
    UI/Modals/FileDialogModal.cpp

    UI/Views/ColorView.h
    UI/Views/ColorView.cpp
    UI/Views/LogView.h
    UI/Views/LogView.cpp
    UI/Views/MaterialView.h
    UI/Views/MaterialView.cpp

    UI/Windows/BaseWindow.h
    UI/Windows/BaseWindow.cpp
    UI/Windows/GameWindow.h
    UI/Windows/GameWindow.cpp
    UI/Windows/HierarchyWindow.h
    UI/Windows/HierarchyWindow.cpp
    UI/Windows/LoggerWindow.h
    UI/Windows/LoggerWindow.cpp
    UI/Windows/PropertyWindow.h
    UI/Windows/PropertyWindow.cpp
    UI/Windows/SceneWindow.h
    UI/Windows/SceneWindow.cpp
    UI/Windows/ProjectWindow.h
    UI/Windows/ProjectWindow.cpp
    UI/Windows/FileExplorerWindow.h
    UI/Windows/FileExplorerWindow.cpp
    UI/Windows/ResourceManagerWindow.h
    UI/Windows/ResourceManagerWindow.cpp
    UI/Windows/MaterialWindow.h
    UI/Windows/MaterialWindow.cpp

    Vendors/stb_image.h
    Vendors/stb_image.cpp

    Vendors/imgui/imconfig.h
    Vendors/imgui/imgui.cpp
    Vendors/imgui/imgui.h
    Vendors/imgui/imgui_demo.cpp
    Vendors/imgui/imgui_draw.cpp
    Vendors/imgui/imgui_internal.h
    Vendors/imgui/imgui_tables.cpp
    Vendors/imgui/imgui_widgets.cpp
    Vendors/imgui/imstb_rectpack.h
    Vendors/imgui/imstb_textedit.h
    Vendors/imgui/imstb_truetype.h
    Vendors/imgui/GraphEditor.cpp
    Vendors/imgui/GraphEditor.h
    Vendors/imgui/ImCurveEdit.cpp
    Vendors/imgui/ImCurveEdit.h
    Vendors/imgui/ImGradient.cpp
    Vendors/imgui/ImGradient.h
    Vendors/imgui/ImGuizmo.cpp
    Vendors/imgui/ImGuizmo.h
    Vendors/imgui/ImSequencer.cpp
    Vendors/imgui/ImSequencer.h
    Vendors/imgui/ImZoomSlider.h
    Vendors/imgui/imgui_impl_opengl3.cpp
    Vendors/imgui/imgui_impl_opengl3.h
    Vendors/imgui/imgui_impl_opengl3_loader.h
    Vendors/imgui/imgui_impl_glfw.cpp
    Vendors/imgui/imgui_impl_glfw.h
)

# Link dependencies
target_link_libraries(Editor PRIVATE
    Profiling
    Core
    glm::glm
    glfw
    glad
    assimp
    nlohmann_json
)

# Include directories
#target_include_directories(Editor PRIVATE
#    ${CMAKE_SOURCE_DIR}/src/Core
#    ${CMAKE_SOURCE_DIR}/src/Vendors/glfw/include
#    ${ASSIMP_DIR}
#    ${JSON_DIR}
#)

# Copy Assets folder
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/Editor/Assets
        $<TARGET_FILE_DIR:Editor>/Assets
)

# Copy imgui.default.ini to root
set(DEFAULT_IMGUI_INI "${CMAKE_SOURCE_DIR}/src/editor/assets/imgui.default.ini")

add_custom_command(TARGET Editor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Editor>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${DEFAULT_IMGUI_INI}" "$<TARGET_FILE_DIR:Editor>/imgui.ini"
  VERBATIM)

# Copy Core.dll after build (needed at runtime)
add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Core>
        $<TARGET_FILE_DIR:Editor>
)

# C++20 support
set_target_properties(Editor PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
