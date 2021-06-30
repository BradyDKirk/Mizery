@ECHO off
REM Compile shaders to SPIR-V

SETLOCAL

REM Paths
set SOURCE_DIR=%~dp0..\source\shaders\
set OUT_DIR=%~dp0..\run_tree\data\shaders\
set VULKAN_BIN=%VULKAN_SDK%\Bin\

REM Create directories
IF NOT EXIST %OUT_DIR% mkdir %OUT_DIR%

REM Compile shaders
echo.
echo Compiling shaders...

echo basic_vert.vert
%VULKAN_BIN%glslangValidator -G %SOURCE_DIR%basic_vert.vert -o %OUT_DIR%basic_vert.spv

echo basic_frag.frag
%VULKAN_BIN%glslangValidator -G %SOURCE_DIR%basic_frag.frag -o %OUT_DIR%basic_frag.spv
echo Done.

ENDLOCAL