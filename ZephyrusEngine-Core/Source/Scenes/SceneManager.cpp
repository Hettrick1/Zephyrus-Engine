#include "SceneManager.h"
#include "SceneFactory.h"
#include <filesystem>
#include "CameraComponent.h"

namespace Zephyrus::Scenes {
	bool SceneManager::mIsSceneLoaded = false;
	Scene* SceneManager::ActiveScene = nullptr;

	void SceneManager::LoadScene(Scene* pScene, bool pCallPostStart)
	{
		mIsSceneLoaded = false;
		Zephyrus::Render::IRenderer* renderer = nullptr;
		if (ActiveScene != nullptr)
		{
			ActiveScene->Close();
			renderer = ActiveScene->GetRenderer();
			delete ActiveScene;
			ActiveScene = nullptr;
		}
		ActiveScene = pScene;
		if (renderer != nullptr)
		{
			ActiveScene->SetRenderer(renderer);
			ActiveScene->Start();
			if (pCallPostStart)
			{
				ActiveScene->PostStart();
			}
		}
	}

	void SceneManager::LoadSplashScreen(Scene* pScene, Zephyrus::Render::IRenderer* pRenderer)
	{
		mIsSceneLoaded = false;
		ActiveScene = pScene;
		ActiveScene->SetRenderer(pRenderer);
		ActiveScene->Start();
		mIsSceneLoaded = true;
	}

	void SceneManager::LoadSceneWithFile(const std::string& pFilePath, Zephyrus::Render::IRenderer* pRenderer, bool pCallPostStart)
	{
		std::string filepath = pFilePath;
		mIsSceneLoaded = false;
		Zephyrus::Render::IRenderer* renderer = pRenderer;
		if (ActiveScene != nullptr)
		{
			ActiveScene->Close();
			renderer = ActiveScene->GetRenderer();
			delete ActiveScene;
			ActiveScene = nullptr;
		}
		std::filesystem::path fsPath(filepath);
		std::string filename = fsPath.filename().string();

		ActiveScene = new Scene(filename);
		if (renderer != nullptr)
		{
			ActiveScene->SetRenderer(renderer);
			ActiveScene->SetFilePath(filepath);
			Zephyrus::Factory::SceneFactory::PopulateSceneFromFile(filepath);
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
		mIsSceneLoaded = true;
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
}
