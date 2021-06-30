#include "cube.h"

namespace Mizery {
    
	ParticleForceRegistry registry;
	ParticleAnchoredBungee* pb;
	ParticleGravity* pg;
	ParticleContactResolver* pcr;
	ParticleContact* pc;
	
    Cube::Cube(Shader* shader, glm::vec3 position, glm::vec3 scale, glm::vec3 color, bool32 isStatic) : shader(shader), position(position), scale(scale), color(color)
    {
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
			particle.position = position;
			particle.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			particle.acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
			particle.setMass(2.0f);
			particle.damping = 0.99f;

			pcr = new ParticleContactResolver(2);
			pc = new ParticleContact();
			pc->contactNormal = glm::vec3(0.0f, 1.0f, 0.0f);
			//pc->penetration = 0.0f;
			pc->restitution = 0.8f;
			pc->particle[0] = &particle;
			pc->particle[1] = NULL;

			// Add gravity
			pg = new ParticleGravity(glm::vec3(0.0f, -10.0f, 0.0f));
		
			// Add spring
			glm::vec3* anchor = new glm::vec3(0.0f, 0.0f, 0.0f);
			pb = new ParticleAnchoredBungee(anchor, 10.0f, 4.0f);

			// Register force generators
			registry.add(&particle, pg);
			registry.add(&particle, pb);
		}
    }
    
    void Cube::update(bool32 isStatic, real32 dt)
    {
		// @TEMP
		if (!isStatic)
		{
			particle.clearAccumulator();
			
			registry.updateForces(dt);
			
			particle.integrate(dt);

			if (particle.position.y < -6.0f)
			{
				pcr->resolveContacts({ pc }, 0, dt);
			}
			position = particle.position;
		}
		
		model = glm::mat4(1.0f);
        model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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