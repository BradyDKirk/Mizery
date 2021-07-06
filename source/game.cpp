#include "game.h"

namespace Mizery {
    
	Shader* basicShader;
    Cube* cube1;
	Cube* cube2;
	Cube* cube3;

	void Game::init()
	{	
		basicShader = new Shader("data/shaders/basic_vert.spv", "data/shaders/basic_frag.spv");

		// Cube(shader, position, scale, color, isStatic)
		cube1 = new Cube(basicShader, glm::vec3( 0.0f, -7.0f, 0.0f), glm::vec3(15.0f, 1.0f, 15.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		cube2 = new Cube(basicShader, glm::vec3(-4.0f,  0.0f, 0.0f), glm::vec3( 1.0f, 1.0f,  1.0f), glm::vec3(1.0f, 0.8f, 0.3f));
		cube3 = new Cube(basicShader, glm::vec3( 0.0f,  0.0f, 0.0f), glm::vec3( 0.1f, 0.1f,  0.1f), glm::vec3(1.0f, 1.0f, 1.0f));
	}

    void Game::processInput(Input& input, real32 dt)
    {
		// Camera movement
		if (input.keyPressed(KEY_W)) { camera->move(FORWARD, dt); }
		if (input.keyPressed(KEY_S)) { camera->move(BACKWARD, dt); }
		if (input.keyPressed(KEY_A)) { camera->move(LEFT, dt); }
		if (input.keyPressed(KEY_D)) { camera->move(RIGHT, dt); }
		if (input.keyPressed(KEY_E)) { camera->move(UP, dt); }
		if (input.keyPressed(KEY_Q)) { camera->move(DOWN, dt); }
		if (input.isMouseMoving()) { camera->updateLook(input.cursorXOffset(), input.cursorYOffset()); }
		
		// @TEMP
		// Reload shaders
		static bool32 rPressed = false;
		if (input.keyPressed(KEY_R) && input.keyPressed(KEY_LEFT_CONTROL))
		{
			if (!rPressed)
			{
				LOG_INFO("Reloading shaders");
				basicShader->reload();
			}
			rPressed = true;
		}
		else
		{
			rPressed = false;
		}
    }
	
    void Game::update(real32 dt)
    {
		cube1->update(dt);
		cube2->update(dt);
		cube3->update(dt);
    }
    
    void Game::render()
    {
        Renderer::instance().startFrame(basicShader); // @TEMP: This should be handled in the engine loop probably

		// Draw cubes
		cube1->draw();
		cube2->draw();
		cube3->draw();
    }
	
	void Game::shutdown()
	{
		LOG_TRACE("Shutting down game...");
	}
    
}