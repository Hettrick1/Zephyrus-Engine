#pragma once
#include <vector>
#include "Scene.h"
#include "ISceneContext.h"

using Zephyrus::Factory::ComponentFactory;
using Zephyrus::Factory::IPrefabFactory;
using Zephyrus::Factory::ISceneFactory;
using Zephyrus::Factory::ISerializationFactory;
/**
 * @brief Manages the current scene, including loading, starting, updating, rendering, and unloading scenes.
 * Provides static methods to control the active scene in the editor or engine.
 */
namespace Zephyrus::Scenes 
{
	class SceneManager : public ISceneContext
	{
	public:

		ComponentFactory* mComponentFactory{ nullptr };
		IPrefabFactory* mPrefabFactory{ nullptr };
		ISceneFactory* mSceneFactory{ nullptr };
		ISerializationFactory* mSerializationFactory{ nullptr };
		Render::IRenderer* mRenderer{ nullptr };

		Scene* ActiveScene;
		bool mIsSceneLoaded;
		SceneManager(Render::IRenderer* pRenderer = nullptr);
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		// Loads the given scene and sets it as the active scene
		void LoadScene(Scene* pScene, bool pCallPostStart = true);
		void LoadSplashScreen(Scene* pScene, Zephyrus::Render::IRenderer* pRenderer);
		void LoadSceneWithFile(const std::string& pFilePath, Zephyrus::Render::IRenderer* pRenderer = nullptr, bool pCallPostStart = true) override;

		// Starts the active scene with the provided renderer
		void StartScene();
		void PostStartScene();

		void Update(float pDetltaTime);
		void RenderAll();

		void BeginRender();
		void RenderScene();
		void EndRender();

		// Returns true if a scene is currently loaded
		bool SceneLoaded();

		// Unloads the current active scene
		void Unload();

		Zephyrus::Physics::PhysicWorld* GetPhysicsWorld() override;
		Zephyrus::Render::IRenderer* GetRenderer() override;
		CameraManager* GetCameraManager() override;
		Zephyrus::Factory::ComponentFactory* GetComponentFactory() override;
		Zephyrus::Factory::IPrefabFactory* GetPrefabFactory() override;
		Zephyrus::Factory::ISceneFactory* GetSceneFactory() override;
		Zephyrus::Scenes::Scene* GetActiveScene() override;
		Zephyrus::Factory::ISerializationFactory* GetSerializationFactory() override;
		void SetSceneLoaded(bool pSceneLoaded) override;
	};
}