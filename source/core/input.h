#pragma once

#include <glfw/glfw3.h>

#include "core/key_codes.h"
#include "core/mouse_codes.h"
#include "core/typedefs.h"
#include "core/log.h"


namespace Mizery {
    
    class Input
    {
    public:
        void init(GLFWwindow* window);
        
        // Keyboard events
        bool32 keyPressed(keyCode code);
        bool32 keyReleased(keyCode code);
        
        // Mouse button events
        bool32 mousePressed(mouseCode code);
        bool32 mouseReleased(mouseCode code);
        
        // Show cursor
        void showCursor();
        
        // Hide cursor
        void hideCursor();
        
        // Getters and setters        
        inline void setCursorLastPos(real32 lastX, real32 lastY) { cursorLastX_ = lastX; cursorLastY_ = lastY; }
        inline real32 cursorLastX() const {return cursorLastX_; }
        inline real32 cursorLastY() const {return cursorLastY_; }
        
        inline void setCursorOffset(real32 xOffset, real32 yOffset) { cursorXOffset_ = xOffset; cursorYOffset_ = yOffset; }
        inline real32 cursorXOffset() const { return cursorXOffset_; }
        inline real32 cursorYOffset() const { return cursorYOffset_; }
        
        inline void setMouseMoving(bool32 moving) { mouseIsMoving_ = moving; }
        inline bool32 isMouseMoving() const { return mouseIsMoving_; }
        
    private:
        GLFWwindow* windowHandle;
        
        real32 cursorLastX_;
        real32 cursorLastY_;
        real32 cursorXOffset_;
        real32 cursorYOffset_;
        bool32 mouseIsMoving_;
        
        static void keyCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods);
        static void mouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods);
        static void cursorPositionCallback(GLFWwindow* window, real64 xpos, real64 ypos);
    
    // Singleton
    public:
        static Input& instance()
        {
            static Input* instance = new Input();
            return *instance;
        }
    private:
        Input() {}
    };
    
}