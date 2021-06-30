#include "window.h"

namespace Mizery {

	// Initialize GLFW, create window, and load OpenGL functions
	Window::Window(int32 width, int32 height, const char* title, bool32 fullscreen, bool32 vsync) : windowWidth(width), windowHeight(height)
	{
		// Init GLFW
		LOG_TRACE("Initializing GLFW...");
		if (!glfwInit())
		{
			LOG_CRITICAL("Failed to initialize GLFW");
			exit(EXIT_FAILURE);
		}
		
		// Log GLFW version
		int glfwMajor, glfwMinor, glfwRev;
		glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRev);
		LOG_INFO("GLFW initialized({0}.{1}.{2})", glfwMajor, glfwMinor, glfwRev);

		// Set GLFW window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 8);
		
		// Create GLFW window
		LOG_TRACE("Creating window...");
		if (fullscreen)
		{
			windowWidth = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
			windowHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
			windowHandle = glfwCreateWindow(windowWidth, windowHeight, title, glfwGetPrimaryMonitor(), nullptr);
		}
		else
		{
			windowHandle = glfwCreateWindow(windowWidth, windowHeight, title, nullptr, nullptr);
		}
		
		if (!windowHandle)
		{
			LOG_CRITICAL("Failed to create window");
			shutdown();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(windowHandle);
		glfwSetWindowUserPointer(windowHandle, this);
		glfwSetFramebufferSizeCallback(windowHandle, framebufferSizeCallback);
		
		LOG_INFO("Window size: {0}x{1}", windowWidth, windowHeight);
		setVsync(vsync);
		
		// Load OpenGL functions
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_CRITICAL("Failed to load OpenGL");
			shutdown();
			exit(EXIT_FAILURE);
		}
		LOG_INFO("Physical device: {0}", glGetString(GL_RENDERER));
		LOG_INFO("OpenGL successfully loaded({0})", glGetString(GL_VERSION));
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
	}
	
	void Window::setVsync(bool32 value)
	{
		// Swap interval of 0 is uncapped framerate where swap interval of 1 is effectively vsync
		glfwSwapInterval(value);
		
		LOG_INFO("vsync = {0}", value);
	}
	
	// Swap buffers and poll events (Should call at end of game loop)
	void Window::update()
	{
		Input::instance().setMouseMoving(false);
		
		// GLFW Swap buffers and poll events
		glfwSwapBuffers(windowHandle);
		glfwPollEvents();
	}

	// Destroy window and terminate GLFW
	void Window::shutdown()
	{
		LOG_TRACE("Destroying window...");
		glfwDestroyWindow(windowHandle);
		glfwTerminate();
	}
	
	// Closes the window
	void Window::closeWindow()
	{
		// Window should close
		glfwSetWindowShouldClose(windowHandle, GLFW_TRUE);
	}

	real32 Window::getTime()
	{
		return glfwGetTime();
	}

	bool32 Window::isMinimized()
	{
		return (windowWidth > 0 || windowHeight > 0) ? false : true;
	}

	// Returns whether the window should close or not
	bool32 Window::shouldWindowClose()
	{
		return glfwWindowShouldClose(windowHandle);
	}

	// Callbacks
	void Window::framebufferSizeCallback(GLFWwindow* window, int32 width, int32 height)
	{
		// Resize OpenGL viewport
		glViewport(0, 0, width, height);
		
		// Set window properties
		Window* appWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		appWindow->windowWidth = width;
		appWindow->windowHeight = height;
	}
	
}