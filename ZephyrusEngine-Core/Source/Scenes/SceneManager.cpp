#include "SceneManager.h"

bool SceneManager::mIsSceneLoaded = false;
Scene* SceneManager::ActiveScene = nullptr;

void SceneManager::LoadScene(Scene* pScene, bool pCallPostStart)
{
	mIsSceneLoaded = false;
	IRenderer* renderer = nullptr;
	if (ActiveScene != nullptr)
	{
		ActiveScene->Close();
		renderer = ActiveScene->GetRenderer();
		delete ActiveScene;
		ActiveScene = nullptr;
	}
	ActiveScene = pScene;
	ActiveScene->Load();
	if (renderer != nullptr)
	{
		ActiveScene->Start(renderer);
		if (pCallPostStart)
		{
			ActiveScene->PostStart();
		}
	}
}

void SceneManager::StartScene(IRenderer* pRenderer)
{
	ActiveScene->Start(pRenderer);
}

void SceneManager::PostStartScene()
{
	ActiveScene->PostStart();
}

void SceneManager::Update()
{
	mIsSceneLoaded = true;
	ActiveScene->Update();
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
}
