#pragma once

#include <glad/glad.h>

#include "core/math.h"
#include "physics/body.h"
#include "renderer/shader.h"

// @FIX: Honestly this whole class is a mess. Just completely redo with the full implementation of the renderer and physics engine

namespace Mizery {
    
    class Cube
    {
    public:
        Cube(Shader* shader, glm::vec3 position, glm::vec3 scale, glm::vec3 color);
        
        void update(real32 dt);
        void draw();
        
    private:
        Shader* shader;
        uint32 cubeVAO;
        
        glm::vec3 color;
        glm::mat4 model;
        
        glm::vec3 position;
        glm::quat orientation;
        glm::vec3 scale;
        
        RigidBody body;
    };
    
}