#include "renderer.h"

namespace Mizery {
    
    void Renderer::init(Window* win)
    {
        LOG_TRACE("Initializing renderer...");
        
        window = win;
        backgroundColor = glm::vec3(0.4f, 0.4f, 0.4f);
        
        updateProjectionMatrix();
        
        camera = new Camera();
    }
    
    void Renderer::startFrame(Shader* shader)
    {
        // Clear color buffer and set background color
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        updateProjectionMatrix();
        
        shader->use();
        shader->setMat4("view", camera->viewMatrix());
		shader->setVec3("viewPos", camera->position);
		shader->setMat4("projection", projectionMatrix);
    }
    
    void Renderer::endFrame()
    {
        // Unbind shaders and vertex arrays
		glUseProgram(0);
		glBindVertexArray(0);
    }
    
    void Renderer::updateProjectionMatrix()
    {
        projectionMatrix = glm::perspective(glm::radians(45.0f), (real32) window->width() / (real32) window->height(), 0.1f, 100.0f);
    }
}