#pragma once

#include "Window.h"
#include "IRenderer.h"
#include "Scene.h"
#include "Timer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Panel/Panel.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Zephyrus::ActorComponent
{
	class EditorControllerActor;
}

namespace Zephyrus::Inputs
{
	class Axis2DAction;
}

using Zephyrus::Inputs::InputManager;

class EditorApplication
{
private:
	SDL_Event mSdlEvent;
	std::string mTitle;
	Window* mGameWindow{ nullptr };
	Zephyrus::Scenes::SceneManager* mSceneManager{ nullptr };
	Zephyrus::Render::IRenderer* mRenderer{ nullptr };
	std::string mStartUpScene;
	InputManager& mInputManager;
	bool mIsRunning{true};
	std::unordered_map<std::string, std::unique_ptr<Panel>> mAllPanels;

	GLuint mFrameBuffer = 0;
	GLuint mRenderTexture = 0;
	GLuint mDepthRenderBuffer = 0;

	Zephyrus::ActorComponent::EditorControllerActor* mEditorController = nullptr;

public:
	EditorApplication(const std::string& pTitle, const std::string& pStartupScene);
	~EditorApplication();

	// Initializes the game systems and window
	void Initialize();
	void InitializeImGui();
	void InitializePanels();

	// Main game loop
	void Loop();

	void Update();
	void Render();
	void RenderImgui();
	void Input();

	void DrawDockSpace();
	void DrawPanels();

	void SetEditorStyle();

	void ResetEditorController();

	// Closes and cleans up the game
	void Close();

public:
	Panel* GetPanelWithName(std::string pPanelName);

public:
	const std::string consolePanelName = "Console";
	const std::string inspectorPanelName = "Inspector";
	const std::string scenePanelName = "Scene";
	const std::string sceneHierarchyName = "Scene Hierarchy";
	const std::string contentBrowserName = "Content Browser";
	const std::string menuPanelName = "MenuPanel";
	const std::string utilsPanelName = "UtilsPanel";
	const std::string prefabPanelName = "Prefabs";
	const float topBarHeight = 45.0f;
};

