@echo off
REM Delete all files and directories generated from the build scripts

SETLOCAL

REM Paths
set WORKING_DIR=%~dp0..\run_tree\
set BUILD_DIR=%~dp0..\build_files\
set SHADER_DIR=%WORKING_DIR%data\shaders\

echo Deleting build files...
REM Delete build files
IF EXIST %BUILD_DIR% rmdir /Q /S %BUILD_DIR%

echo Cleaning working directory...
REM Delete compiled shaders
IF EXIST %SHADER_DIR% rmdir /Q /S %SHADER_DIR%

REM Delete EXE and PDB files
IF EXIST %WORKING_DIR%*.exe del /Q /F %WORKING_DIR%*.exe
IF EXIST %WORKING_DIR%*.pdb del /Q /F %WORKING_DIR%*.pdb

REM Delete ini files
REM IF EXIST %WORKING_DIR%*.ini del /Q /F %WORKING_DIR%*.ini

echo Done.

ENDLOCAL