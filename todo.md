# TODO

## Overall Project
- Rewrite current code in a procedural format rather than OOP with an emphasis on data-oriented design. Look into Mike Acton, Brian Will, Jonathon Blow, and Casey Muratori (to name a few) for reasons that this is a good desicion.

## Build Pipeline
- Look into precompiled headers to improve build time
- Maybe game code should be built in a DLL and imported by the engine?

## Core
- Get rid of GLFW and interface with Windows directly. The only supported platform will be Windows and more control with the input system would be nice

## Audio
- Look into 3D audio APIs (OpenAL, FMOD, Wwise)

## Renderer
- Design a rendering pipeline so that game object can be added easily while still streamlining the rendering process for higher performance rendering
- Shadow mapping
- Add point lights (currently a single light is hardcoded in the fragment shader)
- Down the road think about Forward+ rendering and raytracing (with Vulkan)

## Physics
- Rewrite physics code from scratch taking some inpiration from the code that has already be written