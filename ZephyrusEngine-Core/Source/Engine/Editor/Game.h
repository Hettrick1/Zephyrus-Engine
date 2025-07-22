#pragma once
#include "Window.h"
#include "IRenderer.h"
#include "Scene.h"
#include "Timer.h"
#include "InputManager.h"
#include "Physics/PhysicManager.h"
#include "CameraManager.h"
#include "SceneManager.h"
#include <iostream>
#include <vector>

/**
 * @brief Main class responsible for managing the game loop, window, rendering, input, and scene.
 */
class Game
{
public:
	Game(std::string pTitle, Scene* pStartupScene);
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

private:
	SDL_Event mSdlEvent;
	std::string mTitle;
	Window* mGameWindow = nullptr;
	IRenderer* mRenderer = nullptr;
	Scene* mStartUpScene = nullptr;
	InputManager& mInputManager;
	PhysicManager& mPhysicManager;
	CameraManager& mCameraManager;
	bool mIsRunning = true;
};
