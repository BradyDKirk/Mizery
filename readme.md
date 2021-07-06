# Mizery

Game engine from scratch

## Features and Goals

### Current Features
- Basic core engine features such as window creation, input handling, and logging
- Editor overlay implemented with ImGui (Currently just a demo window)
- Basic OpenGL renderer with shader hotswapping
- Basic rigid body physics engine (Custom implementation to come later)

### Planned Features
- Windows platform layer from scratch (instead of using GLFW)
- Vulkan Forward+ renderer
- Custom rigid body physics engine with collision detection
- 3D audio
- Editor overlay to interface with scenes; edit object properties, place game events, scene loading/saving, etc.

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

## Libraries Used
- [glad](https://github.com/Dav1dde/glad)
- [GLFW](https://github.com/glfw/glfw) (3.3.4)
- [GLM](https://github.com/g-truc/glm) (0.9.9.8)
- [Dear ImGui](https://github.com/ocornut/imgui) (1.83)
- [spdlog](https://github.com/gabime/spdlog) (1.8.5)
- [stb header libraries](https://github.com/nothings/stb) (stb_image.h)
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
- [volk](https://github.com/zeux/volk) (v1.2.175)