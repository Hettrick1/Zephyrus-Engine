#include "pch.h"
#include "Scene.h"

#include "Actor.h"
#include <algorithm>
#include "SceneManager.h"
#include "InputManager.h"
#include "PrefabFactory.h"
#include "Utils/JSONUtils.h"
#include "ActorState.h"
#include "ISceneContext.h"
#include "PlayerStartComponent.h"

using Zephyrus::Assets::AssetsManager;
using Zephyrus::Inputs::InputManager;

namespace Zephyrus::Scenes {
	Scene::Scene(ISceneContext* pContext, std::string pTitle)
		: mContext{ pContext }, mTitle{ pTitle }, mIsUpdatingActor{ false }, 
		mRenderer{ nullptr }, mPhysicWorld{ new PhysicWorld() }, mDebugRenderer{ new PhysicsDebugRenderer(pContext) },
		mCameraManager{ new CameraManager(pContext) }, mInputManager{ new InputManager(pContext->GetRenderer()->GetWindow()->GetGlfwWindow()) }
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
			mPlayerRef = mContext->GetPrefabFactory()->SpawnActorFromPrefab(mContext->GetActiveScene(), doc["player"].GetString());
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
				auto rb = mPlayerRef->GetRigidBody();
				if (rb)
				{
					rb->ForceSyncFromActor();
				}
			}
		}
		else
		{
			mPlayerRef = mContext->GetPrefabFactory()->SpawnActorFromPrefab(mContext->GetActiveScene(),"CameraActor");
			mPlayerRef->SetPosition(Vector3D(0));
			mPlayerRef->SetRotation(Quaternion(0, 0, 0, 0));
			mPlayerRef->SetSize(Vector3D(1));
		}
		mPlayerRef->Start();
		mCameraManager->OnPlay();
		mInputManager->SetPriority();
	}

	void Scene::Update(float pDetltaTime)
	{
		mPhysicWorld->Update(pDetltaTime);
		UpdateAllActors();
	}

	void Scene::Render()
	{
		mCameraManager->RenderActiveCamera();
		mRenderer->RenderActiveCamera(mCameraManager->GetActiveCamera());
	}

	void Scene::Inputs()
	{
		if (!mInputManager->HasPriority())
		{
			return;
		}
		mInputManager->UpdateKeysAndButtons();
	}

	void Scene::SetSceneLoaded(bool pSceneLoaded)
	{
		mIsSceneLoaded = pSceneLoaded;
	}

	void Scene::SetRenderer(Zephyrus::Render::IRenderer* pRenderer)
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
		//InputManager::Instance().Unload();
		mRenderer->Unload();
		AssetsManager::Clear();
		mCameraManager->Unload();
		delete mCameraManager;
		mCameraManager = nullptr;
		delete mDebugRenderer;
		mDebugRenderer = nullptr;
		delete mPhysicWorld;
		mPhysicWorld = nullptr;
		delete mInputManager;
		mInputManager = nullptr;
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
		//InputManager::Instance().Unload();
		mRenderer->Unload();
		mCameraManager->Unload();
		delete mCameraManager;
		mCameraManager = nullptr;
		delete mDebugRenderer;
		mDebugRenderer = nullptr;
		delete mPhysicWorld;
		mPhysicWorld = nullptr;
		delete mInputManager;
		mInputManager = nullptr;
	}

	void Scene::SaveTo(const std::string& pFilePath)
	{
		auto writer = Serialization::Json::JsonWriter();

		std::string playerPrefab = "CameraActor";

		if (mPlayerStart)
		{
			playerPrefab = mPlayerStart->GetComponentOfType<Zephyrus::ActorComponent::PlayerStartComponent>()->GetPlayerPrefabName();
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
				playerPrefab = mPlayerStart->GetComponentOfType<Zephyrus::ActorComponent::PlayerStartComponent>()->GetPlayerPrefabName();
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
		if (!mIsSceneLoaded)
		{
			return;
		}
		mIsUpdatingActor = true;
		for (auto& actor : mAllActors)
		{
			if (!mIsSceneLoaded)
			{
				return;
			}
			actor->Update();
		}
		mIsUpdatingActor = false;
		for (Actor* actor : mPendingActors)
		{
			if (!mIsSceneLoaded)
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
				if (!mIsSceneLoaded)
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
		std::erase(mPendingActors, pActor);
		std::erase(mAllActors, pActor);
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
}
