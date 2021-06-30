@echo off
REM Run game with FPS monitor in title bar

SETLOCAL

set EXE=mizery.exe
IF "%1"=="-d" set EXE=mizery_debug.exe

pushd %~dp0run_tree
IF EXIST %EXE% (
    start %EXE% -hideEditor -fullscreen
) ELSE (
    echo %EXE% not found
)
popd

ENDLOCAL