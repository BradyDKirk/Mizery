@echo off
REM Call vcvarsall.bat to setup the build environment (i.e. cl.exe)

REM Setup build environment
IF NOT DEFINED DevEnvDir call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars64.bat"
