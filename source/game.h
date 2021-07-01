#pragma once

#include "core/input.h"
#include "physics/world.h"
#include "renderer/renderer.h"
#include "utility/log.h"

namespace Mizery {
    
    class Game
    {
    public:
        Game()
        {
            LOG_TRACE("Initializing game...");
            camera = Renderer::instance().getCamera();
            init();
        }
        
        void init();
        void processInput(Input& input, real32 dt);
        void update(real32 dt);
        void render();
        void shutdown();
        
    private:
        Camera* camera;
        World world;
    };
    
}