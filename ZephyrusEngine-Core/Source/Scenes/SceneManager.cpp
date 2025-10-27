#include "SceneManager.h"
#include "SceneFactory.h"
#include "ComponentFactory.h"
#include "PrefabFactory.h"
#include <filesystem>
#include "CameraComponent.h"

namespace Zephyrus::Scenes {
	SceneManager::SceneManager(Render::IRenderer* pRenderer)
		: mRenderer{ pRenderer }
	{
		mComponentFactory = new ComponentFactory();
		mPrefabFactory = new Zephyrus::Factory::PrefabFactory(this, mComponentFactory);;
		mSceneFactory = new Zephyrus::Factory::SceneFactory(this);
		mIsSceneLoaded = false;
		ActiveScene = nullptr;
	}

	void SceneManager::LoadScene(Scene* pScene, bool pCallPostStart)
	{
		mIsSceneLoaded = false;
		mRenderer = nullptr;
		if (ActiveScene != nullptr)
		{
			ActiveScene->Close();
			mRenderer = ActiveScene->GetRenderer();
			delete ActiveScene;
			ActiveScene = nullptr;
		}
		ActiveScene = pScene;
		if (mRenderer != nullptr)
		{
			ActiveScene->SetRenderer(mRenderer);
			ActiveScene->Start();
			if (pCallPostStart)
			{
				ActiveScene->PostStart();
			}
		}
	}

	void SceneManager::LoadSplashScreen(Scene* pScene, Zephyrus::Render::IRenderer* pRenderer)
	{
		ActiveScene = pScene;
		SetSceneLoaded(false);
		ActiveScene->SetRenderer(pRenderer);
		ActiveScene->Start();
		SetSceneLoaded(true);
	}

	void SceneManager::LoadSceneWithFile(const std::string& pFilePath, Zephyrus::Render::IRenderer* pRenderer, bool pCallPostStart)
	{
		std::string filepath = pFilePath;
		SetSceneLoaded(false);
		mRenderer = pRenderer;
		if (ActiveScene != nullptr)
		{
			ActiveScene->Close();
			mRenderer = ActiveScene->GetRenderer();
			delete ActiveScene;
			ActiveScene = nullptr;
		}
		std::filesystem::path fsPath(filepath);
		std::string filename = fsPath.filename().string();

		ActiveScene = new Scene(this, filename);
		if (mRenderer != nullptr)
		{
			ActiveScene->SetRenderer(mRenderer);
			ActiveScene->SetFilePath(filepath);
			mSceneFactory->PopulateSceneFromFile(ActiveScene, filepath);
			ActiveScene->Start();
			if (pCallPostStart)
			{
				ActiveScene->PostStart();
			}
		}
		else
		{
			ZP_CORE_ERROR("No render when loading a scene !");
		}
	}

	void SceneManager::StartScene()
	{
		ActiveScene->Start();
	}

	void SceneManager::PostStartScene()
	{
		ActiveScene->PostStart();
	}

	void SceneManager::Update(float pDetltaTime)
	{
		SetSceneLoaded(true); // TODO : move this in the loadscene when we have the new input system, this will solve a random crash
		ActiveScene->Update(pDetltaTime);
	}

	void SceneManager::RenderAll()
	{
		ActiveScene->Render();
	}

	void SceneManager::BeginRender()
	{
		ActiveScene->BeginRender();
	}

	void SceneManager::RenderScene()
	{
		ActiveScene->RenderCurrentSceneOnly();
	}

	void SceneManager::EndRender()
	{
		ActiveScene->EndRender();
	}

	bool SceneManager::SceneLoaded()
	{
		return mIsSceneLoaded;
	}

	void SceneManager::Unload()
	{
		ActiveScene->Unload();
		delete ActiveScene;
	}
	Zephyrus::Physics::PhysicWorld* SceneManager::GetPhysicsWorld()
	{
		if (ActiveScene)
		{
			return ActiveScene->GetPhysicWorld();
		}
		return nullptr;
	}
	Zephyrus::Render::IRenderer* SceneManager::GetRenderer()
	{
		if (ActiveScene)
		{
			return ActiveScene->GetRenderer();
		}
		else
		{
			return mRenderer;
		}
		return nullptr;
	}
	CameraManager* SceneManager::GetCameraManager()
	{
		if (ActiveScene)
		{
			return ActiveScene->GetCameraManager();
		}
		return nullptr;
	}
	Zephyrus::Factory::ComponentFactory* SceneManager::GetComponentFactory()
	{
		if (mComponentFactory)
		{
			return mComponentFactory;
		}
		return nullptr;
	}
	Zephyrus::Factory::IPrefabFactory* SceneManager::GetPrefabFactory()
	{
		if (mPrefabFactory)
		{
			return mPrefabFactory;
		}
		return nullptr;
	}
	Zephyrus::Factory::ISceneFactory* SceneManager::GetSceneFactory()
	{
		if (mSceneFactory)
		{
			return mSceneFactory;
		}
		return nullptr;
	}
	Zephyrus::Scenes::Scene* SceneManager::GetActiveScene()
	{
		if (ActiveScene)
		{
			return ActiveScene;
		}
		return nullptr;
	}
	void SceneManager::SetSceneLoaded(bool pSceneLoaded)
	{
		mIsSceneLoaded = pSceneLoaded;
		if (ActiveScene)
		{
			ActiveScene->SetSceneLoaded(pSceneLoaded);
		}
	}
}
