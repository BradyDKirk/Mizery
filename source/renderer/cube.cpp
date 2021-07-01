#include "cube.h"

namespace Mizery {
    
	ForceRegistry registry;
	Spring* fgSpring;
	Gravity* fgGravity;
	RigidBody otherBody;
	
    Cube::Cube(Shader* shader, glm::vec3 position, glm::vec3 scale, glm::vec3 color, bool32 isStatic) : shader(shader), position(position), scale(scale), color(color)
    {
		orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        // @TODO: Put this into a CubeModel or something so we aren't duplicating vertex data in memory
		
        real32 vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};
        
        glGenVertexArrays(1, &cubeVAO);
		
		uint32 cubeVBO;
		glGenBuffers(1, &cubeVBO);
		
		glBindVertexArray(cubeVAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(real32), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(real32), (void*)(3 * sizeof(real32)));
		glEnableVertexAttribArray(1);
		
		// @TEMP
		if (!isStatic)
		{
			orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			body.position = position;
			body.orientation = orientation;
			body.velocity = glm::vec3(0.0f, 0.0f, 5.0f);
			body.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			body.acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
			body.setMass(2.0f);
			body.linearDamping = 0.99f;
			body.angularDamping = 0.99f;
			
			// @TODO: Set inertia tensor properly (and learn more about it)
			body.setInertiaTensor(glm::mat3(1.0f));
			
			body.calculateDerivedData();

			otherBody.position = glm::vec3(0.0f);
			otherBody.velocity = glm::vec3(0.0f);
			otherBody.acceleration = glm::vec3(0.0f);
			otherBody.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			otherBody.rotation = glm::vec3(0.0f);
			otherBody.calculateDerivedData();

			// Add gravity
			fgGravity = new Gravity(glm::vec3(0.0f, -10.0f, 0.0f));
		
			// Add spring
			fgSpring = new Spring(glm::vec3(0.5f, 0.0f, 0.0f), &otherBody, glm::vec3(0.0f), 10.0f, 2.0f);

			// Register force generators
			registry.add(&body, fgGravity);
			registry.add(&body, fgSpring);
		}
    }
    
    void Cube::update(bool32 isStatic, real32 dt)
    {
		// @TEMP
		if (!isStatic)
		{
			body.clearAccumulators();
			
			registry.updateForces(dt);
			
			body.integrate(dt);

			position = body.position;
			orientation = body.orientation;
		}
		
		model = glm::mat4(1.0f);
        model = glm::translate(model, position);
		model = model * glm::mat4_cast(orientation);
        model = glm::scale(model, scale);
    }
    
    void Cube::draw()
    {
        shader->use();
        shader->setMat4("model", model);
		shader->setVec3("objectColor", color);
        
        glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
}