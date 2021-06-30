#include "input.h"

namespace Mizery {
    
    void Input::init(GLFWwindow* window)
    {
        windowHandle = window;
        mouseIsMoving_ = false;
        
        showCursor();
        
        // Set GLFW callbacks
        glfwSetKeyCallback(windowHandle, keyCallback);
        glfwSetMouseButtonCallback(windowHandle, mouseButtonCallback);
        glfwSetCursorPosCallback(windowHandle, cursorPositionCallback);
    }
    
    bool32 Input::keyPressed(keyCode code)
    {
        // @TODO: Figure out a way to ignore key repeats
        if (glfwGetKey(windowHandle, (int32)code) == GLFW_PRESS)
        {
            return true;
        }
        return false;
    }
    
    bool32 Input::keyReleased(keyCode code)
    {
        if (glfwGetKey(windowHandle, (int32)code) == GLFW_RELEASE)
        {
            return true;
        }
        return false;
    }
    
    bool32 Input::mousePressed(mouseCode code)
    {
        if (glfwGetMouseButton(windowHandle, (int32)code) == GLFW_PRESS)
        {
            return true;
        }
        return false;
    }
    
    bool32 Input::mouseReleased(mouseCode code)
    {
        if (glfwGetMouseButton(windowHandle, (int32)code) == GLFW_RELEASE)
        {
            return true;
        }
        return false;
    }
    
    void Input::showCursor()
	{
		// Show cursor
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		
		// Compute coordinates for the middle of the screen
        int32 windowWidth, windowHeight;
        glfwGetWindowSize(windowHandle, &windowWidth, &windowHeight);
        
		real64 cursorX = windowWidth / 2.0;
		real64 cursorY = windowHeight / 2.0;
		
		// Set cursor position to middle of the screen
		glfwSetCursorPos(windowHandle, cursorX, cursorY);
		
		// Set new last position to avoid spontaneous movement
		cursorLastX_ = cursorX;
		cursorLastY_ = cursorY;
	}

	void Input::hideCursor()
	{
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
    
    void Input::keyCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods)
    {
        
    }
    
    void Input::mouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods)
    {
        
    }
    
    void Input::cursorPositionCallback(GLFWwindow* window, real64 xpos, real64 ypos)
	{
        // Get reference to input
        Input& input = Input::instance();
        
        // Get last cursor position
		const real32 lastX = input.cursorLastX_;
		const real32 lastY = input.cursorLastY_;

        // Calculate offsets
		const real32 xoffset = (xpos - lastX);
		const real32 yoffset = (lastY - ypos);
		
        // Update state values
        input.cursorLastX_ = xpos;
        input.cursorLastY_ = ypos;
        input.cursorXOffset_ = xoffset;
        input.cursorYOffset_ = yoffset;
		input.mouseIsMoving_ = true;
	}
    
}