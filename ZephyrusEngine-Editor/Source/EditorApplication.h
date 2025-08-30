#pragma once

#include "Window.h"
#include "IRenderer.h"
#include "Scene.h"
#include "Timer.h"
#include "InputManager.h"
#include "Physics/PhysicManager.h"
#include "CameraManager.h"
#include "SceneManager.h"
#include "Panel.h"
#include <iostream>
#include <vector>
#include <memory>

class EditorApplication
{
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
	std::vector<std::unique_ptr<Panel>> mAllPanels;

	GLuint mFrameBuffer = 0;
	GLuint mRenderTexture = 0;
	GLuint mDepthRenderBuffer = 0;

public:
	EditorApplication(const std::string& pTitle, Scene* pStartupScene);
	~EditorApplication();

	// Initializes the game systems and window
	void Initialize();

	void InitializeFrameBuffer();

	// Main game loop
	void Loop();

	void Update();
	void Render();
	void RenderImgui();
	void Input();

	void InitializePanels();

	void DrawDockSpace();
	void DrawPanels();

	void SetEditorStyle();

	// Closes and cleans up the game
	void Close();
};

