#pragma once
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
	class Game
	{
	private:
		SDL_Event mSdlEvent;
		std::string mTitle;
		Window* mGameWindow = nullptr;
		Zephyrus::Render::IRenderer* mRenderer = nullptr;
		std::string mStartUpScene;
		InputManager& mInputManager;
		bool mIsRunning = true;
	public:
		Game(const std::string& pTitle, const std::string& pStartupScene);
		~Game();

		// Initializes the game systems and window
		void Initialize();

		// Main game loop
		void Loop();

		void Update();
		void Render();
		void Input();

		// Closes and cleans up the game
		void Close();
	};
}