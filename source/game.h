#pragma once

#include "core/input.h"
#include "physics/pworld.h"
#include "renderer/renderer.h"
#include "utility/log.h"

namespace Mizery {
    
    class Game
    {
    public:
        Game() : world(10*10)
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
        ParticleWorld world;
    };
    
}