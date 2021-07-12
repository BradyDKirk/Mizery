@echo off
REM Script to build entire project or only the game's source and clean the project

REM Setup environment
set SCRIPTS_DIR=%~dp0scripts\
call %SCRIPTS_DIR%setup_env.bat

SETLOCAL

REM Parse arguments
:ARG_LOOP_START
IF "%1"=="" GOTO ARG_LOOP_END
IF "%1"=="-d" (
    set DEBUG=1
)
IF "%1"=="-a" (
    set BUILD_LIBS=1
    set BUILD_SHADERS=1
)
IF "%1"=="-s" (
    set BUILD_SHADERS=1
    GOTO COMPILE_SHADERS
)
IF "%1"=="-c" (
    %SCRIPTS_DIR%clean.bat
)
IF "%1"=="-h" (
    echo Help:
    echo -d debug
    echo -a build all; libraries, shaders, etc.
    echo -s build shaders
    echo -c clean
    echo -h help
    echo.
    echo Multiple flags can be passed a once:
    echo     'build -a -d' will build everything in debug mode
    echo.
    echo Note: If multiple flags are passed and -s, -c, or -h happen to be one of them then whichever flag
    echo of the three is first will execute and the file will exit

    GOTO:EOF
)
IF NOT "%1"=="" (
    shift /1
    GOTO ARG_LOOP_START
)
:ARG_LOOP_END

REM Build
REM Setup project directories
set WORKING_DIR=.\run_tree\
set BUILD_DIR=.\build_files\
set SOURCE_DIR=.\source\
set VENDOR_DIR=%SOURCE_DIR%..\vendor\

IF DEFINED DEBUG (
    set INT_DIR=%BUILD_DIR%debug\
) ELSE (
    set INT_DIR=%BUILD_DIR%release\
)
IF NOT EXIST %INT_DIR% mkdir %INT_DIR%
IF NOT EXIST %WORKING_DIR% mkdir %WORKING_DIR%

REM Include and library directories
set INCLUDE_DIRS=/I%SOURCE_DIR% /I%VENDOR_DIR% /I%VENDOR_DIR%glad\include\ /I%VENDOR_DIR%glfw\include\ /I%VENDOR_DIR%imgui\ /I%VULKAN_SDK%\include\
set LIB_DIRS=

REM Source files and libraries
set SRC_FILES=%SOURCE_DIR%first.cpp
set LIB_SRC_FILES=%VENDOR_DIR%glfw.c %VENDOR_DIR%vendor_unity.cpp
set LIBS=kernel32.lib user32.lib shell32.lib winmm.lib gdi32.lib

REM Compile and link flags
set COMMON_COMPILE_FLAGS=/nologo /std:c++17 /EHa /Fo%INT_DIR% /DVK_USE_PLATFORM_WIN32_KHR
set COMMON_LINK_FLAGS=/nologo /INCREMENTAL:NO

IF DEFINED DEBUG (
    set COMPILE_FLAGS=/MDd /Od /Zi /DMZ_BUILD_DBG
    set LINK_FLAGS=/DEBUG:FULL /PDB:%WORKING_DIR% /OUT:%WORKING_DIR%mizery_debug.exe
    set PDB_OUT=/Fd%BUILD_DIR%mizery.pdb
    set LIB_PDB_OUT=/Fd%BUILD_DIR%vendor.pdb

    echo.
    echo [DEBUG]:
) ELSE (
    set COMPILE_FLAGS=/MD /O2 /DMZ_BUILD_REL
    set LINK_FLAGS=/OUT:%WORKING_DIR%mizery.exe
    set PDB_OUT=
    set LIB_PDB_OUT=

    echo.
    echo [RELEASE]:
)

REM Build libraries
IF DEFINED BUILD_LIBS (
    echo Building Libraries...
    cl %COMMON_COMPILE_FLAGS% %COMPILE_FLAGS% %LIB_PDB_OUT% %INCLUDE_DIRS% %LIB_SRC_FILES% /c
    echo Done.
    echo.
)

REM Build program
echo Building Mizery...
cl %COMMON_COMPILE_FLAGS% %COMPILE_FLAGS% %PDB_OUT% %INCLUDE_DIRS% %SRC_FILES% /c
cl %INT_DIR%*.obj /link %COMMON_LINK_FLAGS% %LINK_FLAGS% %LIB_DIRS% %LIBS%
echo Done.

REM Compile shaders
:COMPILE_SHADERS
IF DEFINED BUILD_SHADERS (
    call %SCRIPTS_DIR%compile_shaders.bat
)

ENDLOCAL
set SCRIPTS_DIR=
