#include "Scene.h"

#include "Actor.h"
#include "Physics/PhysicManager.h"
#include "CameraManager.h"
#include <algorithm>
#include "SceneManager.h"
#include "InputManager.h"
#include "PrefabFactory.h"
#include "Utils/JSONUtils.h"
#include "ActorState.h"
#include "PlayerStartComponent.h"

Scene::Scene(std::string pTitle) 
	: mTitle(pTitle), mIsUpdatingActor(false), mRenderer(nullptr), mPhysicWorld(new PhysicWorld()), mDebugRenderer(new PhysicsDebugRenderer())
{
	mDebugRenderer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mPhysicWorld->GetWorld()->setDebugDrawer(mDebugRenderer);
}

void Scene::Start()
{
	for (auto& actor : mAllActors)
	{
		actor->Start();
	}
}

void Scene::PostStart()
{
	std::string fullPath = mFilePath;

	std::ifstream file(fullPath);

	if (!file.is_open())
	{
		ZP_CORE_ERROR("Impossible to open the game map file : " + fullPath);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string jsonContent = buffer.str();

	rapidjson::Document doc;
	doc.Parse(jsonContent.c_str());

	if (doc.HasParseError()) {
		ZP_CORE_ERROR("Parsing JSON failed !");
		return;
	}

	if (doc.HasMember("player") && doc["player"].IsString())
	{
		mPlayerRef = PrefabFactory::SpawnActorFromPrefab(doc["player"].GetString());
		if (mPlayerRef)
		{
			if (mPlayerStart)
			{
				mPlayerRef->SetPosition(mPlayerStart->GetTransformComponent().GetPosition());
				mPlayerRef->SetRotation(mPlayerStart->GetTransformComponent().GetRotation());
				mPlayerRef->SetSize(mPlayerStart->GetTransformComponent().GetSize());
				mPlayerStart->SetActive(ActorState::Paused);
			}
			else
			{
				mPlayerRef->SetPosition(Vector3D(0));
				mPlayerRef->SetRotation(Quaternion(0, 0, 0, 0));
				mPlayerRef->SetSize(Vector3D(1));
			}
			mPlayerRef->AddTag("Player");
		}
	}
	else
	{
		mPlayerRef = PrefabFactory::SpawnActorFromPrefab("CameraActor");
		mPlayerRef->SetPosition(Vector3D(0));
		mPlayerRef->SetRotation(Quaternion(0, 0, 0, 0));
		mPlayerRef->SetSize(Vector3D(1));
	}
	mPlayerRef->Start();

	mPhysicWorld->Test();
}

void Scene::Update()
{
	mPhysicWorld->Update();
	UpdateAllActors();
}

void Scene::Render()
{
	mRenderer->BeginDraw();
	mRenderer->Draw();
	mPhysicWorld->GetWorld()->debugDrawWorld();
	mDebugRenderer->FlushDraw();
	mRenderer->EndDraw();
}

void Scene::SetRenderer(IRenderer* pRenderer)
{
	mRenderer = pRenderer;
}

void Scene::BeginRender()
{
	mRenderer->BeginDraw();
}

void Scene::RenderCurrentSceneOnly()
{
	mRenderer->Draw();
}

void Scene::EndRender()
{
	mRenderer->EndDraw();
}

void Scene::Unload()
{
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
	mActors.clear();
	InputManager::Instance().Unload();
	mRenderer->Unload();
	PhysicManager::Instance().Unload();
	CameraManager::Instance().Unload();
	Assets::Clear();
	delete mPhysicWorld;
	mPhysicWorld = nullptr;
	delete mDebugRenderer;
	mDebugRenderer = nullptr;
}

void Scene::Close()
{
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
	mActors.clear();
	InputManager::Instance().Unload();
	mRenderer->Unload();
	PhysicManager::Instance().Unload();
	CameraManager::Instance().Unload();
	delete mPhysicWorld;
	mPhysicWorld = nullptr;
	delete mDebugRenderer;
	mDebugRenderer = nullptr;
}

void Scene::SaveTo(const std::string& pFilePath)
{
	auto writer = Serialization::Json::JsonWriter();

	std::string playerPrefab = "CameraActor";

	if (mPlayerStart)
	{
		playerPrefab = mPlayerStart->GetComponentOfType<PlayerStartComponent>()->GetPlayerPrefabName();
	}

	writer.WriteString("player", playerPrefab);

	writer.BeginArray("actors");
	for (auto& actor : mAllActors)
	{
		actor->Serialize(writer);
	}
	writer.EndArray();

	writer.SaveDocument(pFilePath);
	mSaved = true;
}

void Scene::SetPlayerStart(Actor* pPlayerStart)
{
	mPlayerStart = pPlayerStart;
}

void Scene::SetFilePath(const std::string& pFilePath)
{
	mFilePath = pFilePath;
}

void Scene::SaveScene()
{
	if (!mFilePath.empty())
	{
		auto writer = Serialization::Json::JsonWriter();

		std::string playerPrefab = "CameraActor";

		if (mPlayerStart)
		{
			playerPrefab = mPlayerStart->GetComponentOfType<PlayerStartComponent>()->GetPlayerPrefabName();
		}

		writer.WriteString("player", playerPrefab);

		writer.BeginArray("actors");
		for (auto& actor : mAllActors)
		{
			actor->Serialize(writer);
		}
		writer.EndArray();

		writer.SaveDocument(mFilePath);
		mSaved = true;
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
		if (mActors.find(pActor->GetUUID()) == mActors.end() && pActor->GetUUID() != "")
		{
			mActors[pActor->GetUUID()] = pActor;
		}
	}
}

void Scene::UpdateAllActors()
{
	if (!SceneManager::mIsSceneLoaded)
	{
		return;
	}
	mIsUpdatingActor = true;
	for (auto& actor : mAllActors)
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
		AddActor(actor);
	}
	mPendingActors.clear();
	for (auto& actor : mAllActors)
	{
		if (actor->GetState() == ActorState::Dead)
		{
			if (!SceneManager::mIsSceneLoaded)
			{
				return;
			}
			RemoveActor(actor);
		}
	}
}

void Scene::RemoveActor(Actor* pActor)
{
	RemoveActorWithID(pActor->GetUUID());
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

void Scene::RemoveActorWithID(const std::string& pId)
{
	auto it = mActors.find(pId);
	if (it != mActors.end())
	{
		mActors.erase(it);
	}
}

Actor* Scene::GetActorWithID(const std::string& pID)
{
	auto it = mActors.find(pID);
	if (it != mActors.end())
	{
		return it->second;
	}
	return nullptr;
}
