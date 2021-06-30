// GLFW sources

// Compilation Flags
#define _GLFW_WIN32

// Common
#include <glfw/src/context.c>
#include <glfw/src/init.c>
#include <glfw/src/input.c>
#include <glfw/src/monitor.c>
#include <glfw/src/vulkan.c>
#include <glfw/src/window.c>

// Windows
#include <glfw/src/win32_init.c>
#include <glfw/src/win32_joystick.c>
#include <glfw/src/win32_monitor.c>
#include <glfw/src/win32_time.c>
#include <glfw/src/win32_thread.c>
#include <glfw/src/win32_window.c>

// Other required sources
#include <glfw/src/wgl_context.c>
#include <glfw/src/egl_context.c>
#include <glfw/src/osmesa_context.c>
