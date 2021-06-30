// Unity build
#include "core/typedefs.h"

#include "core/engine.cpp"
#include "core/input.cpp"
#include "core/window.cpp"

#include "editor/editor.cpp"

#include "physics/particle.cpp"
#include "physics/pcontacts.cpp"
#include "physics/pfgen.cpp"
#include "physics/plinks.cpp"
#include "physics/pworld.cpp"

#include "renderer/camera.cpp"
#include "renderer/cube.cpp"
#include "renderer/renderer.cpp"
#include "renderer/shader.cpp"

#include "utility/log.cpp"

#include "game.cpp"

using namespace Mizery;

// Parse command line arguments and setup CommandLineArgs struct for use in engine
void parseCommandLineArgs(int32 argc, char** argv, CommandLineArgs& args)
{
	// Parse arguments if arguments were passed
	if (argc >= 2)
	{
		for (int32 i = 1; i < argc; i++)
		{
			// Get next token
			const char* token = argv[i];
			
			// Settings list
			if (!strcmp(token, "-fullscreen")) { args.fullscreen = true; }  // fullscreen mode
			if (!strcmp(token, "-vsync")) { args.vsync = true; }            // vsync
			if (!strcmp(token, "-hideEditor")) { args.showEditor = false; } // hides the editor on startup
		}
	}
	
	// Print command line args
	LOG_INFO("Launching with settings:");
	LOG_INFO("\targs.fullscreen = {0}", args.fullscreen);
	LOG_INFO("\targs.vsync = {0}", args.vsync);
	LOG_INFO("\targs.showEditor = {0}", args.showEditor);
}

int main(int argc, char **argv)
{	
	Log::init();
	
	// Parse command line args
	CommandLineArgs clArgs;
	parseCommandLineArgs(argc, argv, clArgs);
	
	// Start engine
	Engine* engine = new Engine(clArgs);
	engine->run();
	delete engine;
	
	return 0;
}
