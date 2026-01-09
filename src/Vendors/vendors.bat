@echo off
REM Batch script to clone multiple Git repositories into src/vendors folder

git clone https://github.com/glfw/glfw.git
git clone https://github.com/g-truc/glm.git
git clone https://github.com/assimp/assimp.git
git clone https://github.com/nlohmann/json.git
git clone https://github.com/mikke89/RmlUi.git
git clone https://github.com/freetype/freetype.git

echo All repositories cloned successfully.
exit /b 0
