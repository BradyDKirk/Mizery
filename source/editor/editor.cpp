#include "editor.h"

namespace Mizery {

	Editor::Editor(Window* window, bool32 showEditor) : window(window), showEditor(showEditor)
	{
		LOG_TRACE("Initializing editor...");
		
		// ImGui init
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		
		// Disable imgui.ini file
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		
		ImGui_ImplGlfw_InitForOpenGL(window->handle(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
		
		// Show cursor is editor is showing
		if (showEditor)
		{
			Input::instance().showCursor();
		}
		else
		{
			Input::instance().hideCursor();
		}
	}

	void Editor::processInput(Input& input)
	{
		// Render in wireframe mode or normal mode
		if (input.keyPressed(KEY_1))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (input.keyPressed(KEY_2))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}

	void Editor::render()
	{
		// ImGui Draw
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		if (showEditor)
		{
			ImGui::ShowDemoWindow();
		}
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Editor::toggleEditor()
	{
		showEditor = !showEditor;
		if (showEditor)
		{
			Input::instance().showCursor();
		}
		else
		{
			Input::instance().hideCursor();
		}
	}

	void Editor::shutdown()
	{
		LOG_TRACE("Shutting down editor...");
		
		// ImGui cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}