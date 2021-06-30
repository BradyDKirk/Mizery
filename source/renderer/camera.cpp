#include "camera.h"

namespace Mizery {
    
    Camera::Camera()
    {
        yaw = -90.0f;
        pitch = -10.0f;
        
        movementSpeed = 3.0f;
        mouseSensitivity = 0.1f;
        
        position = glm::vec3(0.0f, 0.0f, 18.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        
        updateVectors();
    }
    
    void Camera::move(CameraMovementDirection direction, real32 dt)
    {
        const real32 speed = movementSpeed * dt;
        if (direction == FORWARD)  { position += front * speed; }
        if (direction == BACKWARD) { position -= front * speed; }
        if (direction == LEFT)     { position -= right * speed; }
        if (direction == RIGHT)    { position += right * speed; }
        if (direction == UP)       { position += up * speed; }
        if (direction == DOWN)     { position -= up * speed; }
    }
    
    void Camera::updateLook(real32 xoffset, real32 yoffset)
    {
        // Calculate new yaw and pitch
        yaw += xoffset * mouseSensitivity;
        pitch += yoffset * mouseSensitivity;
        
        // Constraint pitch so you can't flip camera over
        if (pitch > 89.0f) { pitch = 89.0f; }
        if (pitch < -89.0f) { pitch = -89.0f; }
        
        updateVectors();
    }
    
    void Camera::updateVectors()
    {
        const real32 yawRad = glm::radians(yaw);
        const real32 pitchRad = glm::radians(pitch);
        
        glm::vec3 frontTemp;
		frontTemp.x = cos(yawRad) * cos(pitchRad);
		frontTemp.y = sin(pitchRad);
		frontTemp.z = sin(yawRad) * cos(pitchRad);
		front = glm::normalize(frontTemp);
		
		right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
		up = glm::normalize(glm::cross(right, front));
    }
    
}