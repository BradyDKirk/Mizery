# Mizery

P.T. style horror game with custom built engine

## Building the Project

### Requirements
- [Visual Studio 2019](https://visualstudio.microsoft.com/) or newer: Required for library and source compilation
- [Vulkan SDK](https://vulkan.lunarg.com/) by LunarG: Required for shader compilation
- Graphics card supporting OpenGL 4.6

**Note: Mizery will be moving to Vulkan entirely in the future so the Vulkan SDK will be required regardless**

### Building Mizery
When building for the first time or building after cleaning, run `build -a` to build the entire project (libraries, source, shaders, etc.). For subsequent builds, run `build`. To look at the build scripts full usage, run `build -h`.

**Note: When building for debug append `-d` to the above commands.**

### Shader Compilation
Mizery loads precompiled SPIR-V shaders rather than compiling shader source at runtime. This means that shaders need to be compiled manually whenever they are edited. To compile shaders, run `build -s`.

## Running
To run, just call `run` for release build or `run -d` for debug build.`run.bat` may be modified to pass in additional arguments to the engine.

Valid arguments are listed below:
- `-fullscreen`: Launch in fullscreen mode
- `-vsync`: Enable vsync
- `-hideEditor`: Hide editor on startup