#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "Window.h"
#include "IRenderer.h"
#include "Scene.h"
#include "Timer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Panel/Panel.h"
#include "ImGuiEditorLayer.h"
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
	std::string mTitle;
	Window* mGameWindow{ nullptr };
	Zephyrus::Scenes::SceneManager* mSceneManager{ nullptr };
	Zephyrus::Render::IRenderer* mRenderer{ nullptr };
	std::string mStartUpScene;
	InputManager& mInputManager;
	bool mIsRunning{true};

	Zephyrus::ActorComponent::EditorControllerActor* mEditorController{ nullptr };

	std::unique_ptr<ImGuiEditorLayer> mImGuiEditorLayer{ nullptr };

public:
	EditorApplication(const std::string& pTitle, const std::string& pStartupScene);
	~EditorApplication();

	// Initializes the game systems and window
	void Initialize();

	// Main game loop
	void Loop();

	void Update();
	void Render();
	void Input();

	void ResetEditorController();

	// Closes and cleans up the game
	void Close();

public:
	inline Zephyrus::Scenes::SceneManager* GetSceneManager() const { return mSceneManager; }
	inline Zephyrus::ActorComponent::EditorControllerActor* GetEditorController() const { return mEditorController; }
};

