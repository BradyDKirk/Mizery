#pragma once

#include <glad/glad.h>

#include "core/log.h"
#include "core/math.h"
#include "core/window.h"
#include "renderer/camera.h"
#include "renderer/cube.h"
#include "renderer/shader.h"

namespace Mizery {
    
    class Renderer
    {
    public:
        // Initializes the renderer
        void init(Window* window);
    
        // Should be called before rendering anything
        void startFrame(Shader* shader);
        
        // Should be called at end of rendering loop
        void endFrame();
    
        // Return pointer to the camera
        Camera* getCamera() { return camera; }
    
    private:
        Window* window;
        Camera* camera;
        
        glm::vec3 backgroundColor;
        glm::mat4 projectionMatrix;
    
        // Updates the projection matrix given the window size
        void updateProjectionMatrix();
        
    // Singleton
    public:
        static Renderer& instance()
        {
            static Renderer* instance = new Renderer();
            return *instance;
        }
    private:
        Renderer() {}
    };
    
}