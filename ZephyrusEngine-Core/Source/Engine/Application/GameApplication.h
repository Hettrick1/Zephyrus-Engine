#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "Window.h"
#include "IRenderer.h"
#include "Scene.h"
#include "Timer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include <iostream>
#include <vector>

/**
 * @brief Main class responsible for managing the game loop, window, rendering, input, and scene.
 */
using Zephyrus::Inputs::InputManager;

namespace Zephyrus::Application {
	class GameApplication
	{
	private:
		std::string mProjectName;
		Window* mGameWindow{ nullptr };
		Zephyrus::Scenes::SceneManager* mSceneManager{ nullptr };
		Zephyrus::Render::IRenderer* mRenderer{ nullptr };
		std::string mEditorConfigFile;
		std::string mStartMapId;
		bool mIsRunning = true;
	public:
		GameApplication(const std::string& pTitle, const std::string& pGameConfigFile);
		~GameApplication();

		// Initializes the game systems and window
		void Initialize();
		void InitializeEditorConfig();

		// Main game loop
		void Loop();

		void Update();
		void Render();
		void Input();

		// Closes and cleans up the game
		void Close();
	};
}