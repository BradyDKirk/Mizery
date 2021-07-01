#pragma once

#include "core/log.h"
#include "core/typedefs.h"
#include "core/window.h"
#include "editor/editor.h"
#include "renderer/renderer.h"

#include "game.h"

namespace Mizery {

    struct CommandLineArgs
    {
        // Window flags
        bool32 fullscreen = false;
        bool32 vsync = false;
        
        // Editor flags
        bool32 showEditor = true;
    };

    class Engine
    {
    public:
        Engine(CommandLineArgs& args) : clArgs(args) {}

        // Runs the engine; processing input, update game state, render, etc.
        void run();
        
    private:
        void processGlobalInput(Input& input);
    
        // Initialize and create a window, load OpenGL functions, initialize UI
        void startup();
        
        // Destroy and terminate any remaining objects
        void shutdown();
        
        CommandLineArgs clArgs;
        
        Window* window;
        Editor* editor;
        Game* game;
    };

}