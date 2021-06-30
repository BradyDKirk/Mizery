@echo off
REM Call vcvars64.bat to setup the build environment (i.e. cl.exe)

REM Find vcvars64.bat file and run it
IF NOT DEFINED DevEnvDir (
    IF EXIST "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars64.bats" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvars64.bat"    
    ) ELSE IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bats" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) ELSE (
        echo "Unable to find vcvars64.bat, make sure you have Visual Studio 2019 or later installed"
    )
)