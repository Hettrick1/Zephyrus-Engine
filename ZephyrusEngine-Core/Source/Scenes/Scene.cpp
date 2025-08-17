#include "Scene.h"

#include "Actor.h"
#include "Physics/PhysicManager.h"
#include "CameraManager.h"
#include <algorithm>
#include "SceneManager.h"
#include "InputManager.h"


Scene::Scene(std::string pTitle) 
	: mTitle(pTitle), mIsUpdatingActor(false), mRenderer(nullptr)
{
}

void Scene::Start(IRenderer* pRenderer)
{
	mRenderer = pRenderer;
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
	Assets::Clear();
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

void Scene::AddActor(Actor* pActor)
{
	pActor->AttachScene(*this);
	if (mIsUpdatingActor) 
	{ 
		mPendingActors.emplace_back(pActor); 
	}
	else
	{
		mAllActors.emplace_back(pActor);
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

void Scene::RemoveActor(Actor* pActor)
{
	std::vector<Actor*>::iterator it = find(mPendingActors.begin(), mPendingActors.end(), pActor); 
	if (it != mPendingActors.end())
	{
		iter_swap(it, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	it = find(mAllActors.begin(), mAllActors.end(), pActor);
	if (it != mAllActors.end())
	{
		iter_swap(it, mAllActors.end() - 1);
		mAllActors.pop_back(); 
	}

}
