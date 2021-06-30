#include "engine.h"

namespace Mizery {

	// Engine startup and initialization code
	void Engine::startup()
	{
		LOG_INFO("Starting Mizery Engine");
		
		// Create window
		window = new Window(1600, 900, "Mizery Game", clArgs.fullscreen, clArgs.vsync);
	
		// Initialize engine systems
		Input::instance().init(window->handle());
		Renderer::instance().init(window);
		
		// Initialize UI
		editor = new Editor(window, clArgs.showEditor);
		
		// Initialize game
		game = new Game();
	}

	// Engine update and render
	void Engine::run()
	{
		// Initialize engine and create window
		startup();
		
		// References to engine singletons
		Input& input = Input::instance();
		Renderer& renderer = Renderer::instance();
		
		// Used for calculating dt
		float currentTime = 0.0f;
		float deltaTime = 0.0f;
		float lastTime = 0.0f;

		// Game Loop
		while (!window->shouldWindowClose())
		{
			// Calculate dt
			currentTime = window->getTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;
			
			// Only run game and editor when window isn't minimized
			if (!window->isMinimized())
			{
				// Input processing for entire program regardless of whether the game is running or not
				processGlobalInput(input);
				
				// Input processing for game and editor
				if (editor->isShowingEditor())
				{
					editor->processInput(input);
				}
				else
				{
					game->processInput(input, deltaTime);
				}
			
				// Update game state
				game->update(deltaTime);
				
				// Render game and editor
				game->render();
				renderer.endFrame();
				
				editor->render();
			}
			
			// Swap buffers and poll for events
			window->update();
		}
		
		// Cleanup engine resources
		shutdown();
	}

	void Engine::processGlobalInput(Input& input)
	{	
		// Toggle editor
		static bool tabPressed = false;
		if (input.keyPressed(KEY_TAB) && input.keyPressed(KEY_LEFT_CONTROL))
		{
			if (!tabPressed)
			{
				editor->toggleEditor();
			}
			tabPressed = true;
		}
		else
		{
			tabPressed = false;
		}
	}

	// Engine shutdown code
	void Engine::shutdown()
	{
		LOG_INFO("Shutting down Mizery Engine");
		
		game->shutdown();
		editor->shutdown();
		window->shutdown();
	}

}