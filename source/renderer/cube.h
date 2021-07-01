#pragma once

#include <glad/glad.h>

#include "core/math.h"
#include "physics/fgen.h"
#include "renderer/shader.h"

// @FIX: Honestly this whole class is a mess. Just completely redo with the full implementation of the renderer and physics engine

namespace Mizery {
    
    class Cube
    {
    public:
        Cube(Shader* shader, glm::vec3 position, glm::vec3 scale, glm::vec3 color, bool32 isStatic);
        
        void update(bool32 isStatic, real32 dt);
        void draw();
        
    private:
        Shader* shader;
        glm::vec3 color;
        glm::vec3 position;
        glm::quat orientation;
        glm::vec3 scale;
        glm::mat4 model;
        uint32 cubeVAO;
        RigidBody body;
    };
    
}