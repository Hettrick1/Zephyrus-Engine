#pragma once
#include <vector>
#include "Scene.h"

/**
 * @brief Manages the current scene, including loading, starting, updating, rendering, and unloading scenes.
 * Provides static methods to control the active scene in the editor or engine.
 */
class SceneManager
{
public:
	static Scene* ActiveScene;	
	static bool mIsSceneLoaded;
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	// Loads the given scene and sets it as the active scene
	static void LoadScene(Scene* pScene);

	// Starts the active scene with the provided renderer
	static void StartScene(IRenderer* pRenderer);
	static void PostStartScene();

	static void Update();
	static void Render();

	// Returns true if a scene is currently loaded
	static bool SceneLoaded();

	// Unloads the current active scene
	static void Unload();
};