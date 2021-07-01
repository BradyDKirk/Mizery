#pragma once

#include "core/log.h"
#include "core/math.h"
#include "core/window.h"

namespace Mizery {
    
    enum CameraMovementDirection
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    
    class Camera
    {
    public:
        // Camera properties
        real32 yaw;
	    real32 pitch;
        real32 movementSpeed;
        real32 mouseSensitivity;
        
        glm::vec3 position;
	    glm::vec3 front;
	    glm::vec3 up;
        glm::vec3 right;
        
        Camera();
        
        // Camera movement
        void move(CameraMovementDirection direction, real32 dt);
        void updateLook(real32 xoffset, real32 yoffset);
        
        // Return view matrix
        glm::mat4 viewMatrix() const { return glm::lookAt(position, position + front, up); }
        
    private:
        // Updates camera orientation vectors
        void updateVectors();
    };
    
}