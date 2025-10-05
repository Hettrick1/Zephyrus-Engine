#pragma once
#include <vector>
#include "Scene.h"
#include "ISceneContext.h"

using Zephyrus::Factory::ComponentFactory;
using Zephyrus::Factory::PrefabFactory;
using Zephyrus::Factory::SceneFactory;

/**
 * @brief Manages the current scene, including loading, starting, updating, rendering, and unloading scenes.
 * Provides static methods to control the active scene in the editor or engine.
 */
namespace Zephyrus::Scenes 
{
	class SceneManager : public ISceneContext
	{
	public:

		static ComponentFactory* mComponentFactory;
		static PrefabFactory* mPrefabFactory;
		static SceneFactory* mSceneFactory;

		static Scene* ActiveScene;
		static bool mIsSceneLoaded;
		SceneManager() = default;
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		// Loads the given scene and sets it as the active scene
		static void LoadScene(Scene* pScene, bool pCallPostStart = true);
		static void LoadSplashScreen(Scene* pScene, Zephyrus::Render::IRenderer* pRenderer);
		static void LoadSceneWithFile(const std::string& pFilePath, Zephyrus::Render::IRenderer* pRenderer = nullptr, bool pCallPostStart = true);

		// Starts the active scene with the provided renderer
		static void StartScene();
		static void PostStartScene();

		static void Update(float pDetltaTime);
		static void RenderAll();

		static void BeginRender();
		static void RenderScene();
		static void EndRender();

		// Returns true if a scene is currently loaded
		static bool SceneLoaded();

		// Unloads the current active scene
		static void Unload();

		Zephyrus::Physics::PhysicWorld* GetPhysicsWorld() override;
		Zephyrus::Render::IRenderer* GetRenderer() override;
		CameraManager* GetCameraManager() override;
		Zephyrus::Factory::ComponentFactory* GetComponentFactory() override;
		Zephyrus::Factory::PrefabFactory* GetPrefabFactory() override;
		Zephyrus::Factory::SceneFactory* GetSceneFactory() override;
		Zephyrus::Scenes::Scene* GetActiveScene() override;
	};
}