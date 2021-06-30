#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "core/typedefs.h"
#include "core/input.h"
#include "utility/log.h"

namespace Mizery {

    class Window
    {
    public:
        // Initialize GLFW, create window, and load OpenGL functions
        Window(int32 width, int32 height, const char* title, bool32 fullscreen = false, bool32 vsync = false);
        
        // If value is true turn on vsync, if false turn off vsync
        void setVsync(bool32 value);
        
        // Swap buffers and poll events (Should call at end of game loop)
        void update();
        
        // Destroy window and terminate GLFW
        void shutdown();
        
        // Closes the window
        inline void closeWindow();
        
        // Returns the time
        inline real32 getTime();
        
        // Returns wheter the window is minimized or not
        inline bool32 isMinimized();
        
        // Returns whether the window should close or not
        inline bool32 shouldWindowClose();
        
        // Getters
        int32 width() const { return windowWidth; }
        int32 height() const { return windowHeight; }
        GLFWwindow* handle() const { return windowHandle; }
        
    private:
        int32 windowWidth;
        int32 windowHeight;
        GLFWwindow* windowHandle;
        
        // Callbacks
        static void framebufferSizeCallback(GLFWwindow* window, int32 width, int32 height);
    };

}