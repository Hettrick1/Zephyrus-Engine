#include "Scene.h"

#include "Actor.h"
#include "Physics/PhysicManager.h"
#include "CameraManager.h"
#include <algorithm>
#include "SceneManager.h"
#include "InputManager.h"


Scene::Scene(std::string title) 
	: mTitle(title), mIsUpdatingActor(false), mRenderer(nullptr)
{
}

void Scene::Start(IRenderer* renderer)
{
	mRenderer = renderer;
}

void Scene::Load()
{
}

void Scene::Update()
{
	UpdateAllActors();
}

void Scene::Render()
{
	mRenderer->BeginDraw();
	mRenderer->Draw();
	mRenderer->EndDraw();
}

void Scene::Unload()
{
	InputManager::Instance().Unload();
	mRenderer->Unload();
	PhysicManager::Instance().Unload();
	CameraManager::Instance().Unload();
	while (!mAllActors.empty()) {
		mAllActors.back()->Destroy();
		delete mAllActors.back();
		mAllActors.pop_back();
	}
	while (!mPendingActors.empty()) {
		mPendingActors.back()->Destroy();
		delete mPendingActors.back();
		mAllActors.pop_back();
	}
	Assets::Clear();
}

void Scene::Close()
{
	InputManager::Instance().Unload();
	mRenderer->Unload();
	PhysicManager::Instance().Unload();
	CameraManager::Instance().Unload();
	while (!mAllActors.empty()) {
		mAllActors.back()->Destroy();
		delete mAllActors.back();
		mAllActors.pop_back();
	}
	while (!mPendingActors.empty()) {
		mPendingActors.back()->Destroy();
		delete mPendingActors.back();
		mAllActors.pop_back();
	}
}

void Scene::AddActor(Actor* actor)
{
	actor->AttachScene(*this);
	if (mIsUpdatingActor) 
	{ 
		mPendingActors.emplace_back(actor); 
	}
	else
	{
		mAllActors.emplace_back(actor);
	}
}

void Scene::UpdateAllActors()
{
	if (!SceneManager::mIsSceneLoaded)
	{
		return;
	}
	mIsUpdatingActor = true;
	for (Actor* actor : mAllActors) 
	{
		if (!SceneManager::mIsSceneLoaded)
		{
			return;
		}
		actor->Update();
	}
	mIsUpdatingActor = false;
	for (Actor* actor : mPendingActors) 
	{
		if (!SceneManager::mIsSceneLoaded)
		{
			return;
		}
		mAllActors.emplace_back(actor);
	}
	mPendingActors.clear();
	for (Actor* actor : mAllActors) 
	{
		if (actor->GetState() == ActorState::Dead)
		{
			if (!SceneManager::mIsSceneLoaded)
			{
				return;
			}
			delete actor;
		}
	}
}

void Scene::RemoveActor(Actor* actor)
{
	std::vector<Actor*>::iterator it = find(mPendingActors.begin(), mPendingActors.end(), actor); 
	if (it != mPendingActors.end())
	{
		iter_swap(it, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	it = find(mAllActors.begin(), mAllActors.end(), actor);
	if (it != mAllActors.end())
	{
		iter_swap(it, mAllActors.end() - 1);
		mAllActors.pop_back(); 
	}

}
