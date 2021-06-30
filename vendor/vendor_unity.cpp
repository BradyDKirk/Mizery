// All vendor source code for a unity build of the C++ libraries
// Excludes GLFW because GLFW must be built in C

// Vendor Source
// =============

// Volk
#include <volk/volk.c>

// Glad
#include <glad/src/glad.c>

// ImGui
#include <imgui/imgui.cpp>
#include <imgui/imgui_demo.cpp>
#include <imgui/imgui_draw.cpp>
#include <imgui/imgui_tables.cpp>
#include <imgui/imgui_widgets.cpp>
#include <imgui/imgui_impl_glfw.cpp>
#include <imgui/imgui_impl_opengl3.cpp>

// STB
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// TinyObjLoader
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>
