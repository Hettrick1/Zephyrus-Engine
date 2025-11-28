#pragma once
#include "IRenderer.h"
#include <vector>
#include <unordered_map>
#include "AssetsManager.h"
#include "Physics/Bullet/PhysicWorld.h"
#include "Physics/Bullet/PhysicsDebugRenderer.h"
#include "CameraManager.h"

namespace Zephyrus::ActorComponent
{
	class Actor;
}

class ISceneContext;

/**
 * @brief Represents a scene containing actors and manages their lifecycle.
 * Handles initialization, loading, updating, rendering, unloading, and closing of the scene.
 * Also manages the addition and removal of actors.
 */

using Zephyrus::ActorComponent::Actor;
using Zephyrus::ActorComponent::ActorState;
using Zephyrus::Physics::PhysicsDebugRenderer;
using Zephyrus::Physics::PhysicWorld;

namespace Zephyrus::Scenes {
	class Scene
	{
	protected:
		ISceneContext* mContext{ nullptr };
		std::string mTitle;
		std::string mFilePath;
		std::string mPlayerPrefabName;
		Zephyrus::Render::IRenderer* mRenderer;
		std::unordered_map<std::string, Actor*> mActors;
		std::vector<Actor*> mAllActors;
		std::vector<Actor*> mPendingActors;
		bool mIsUpdatingActor;
		bool mIsSceneLoaded{ false };
		Actor* mPlayerRef{ nullptr };
		Actor* mPlayerStart{ nullptr };
		bool mSaved{ true };
		PhysicWorld* mPhysicWorld{ nullptr };
		PhysicsDebugRenderer* mDebugRenderer{ nullptr };
		CameraManager* mCameraManager{ nullptr };
	public:
		Scene(ISceneContext* pContext, std::string pTitle = "Scene");
		virtual void Start(); // Initializes the scene with the given renderer
		virtual void PostStart();
		virtual void Update(float pDetltaTime); // Updates the scene and its actors
		virtual void Render();
		void SetSceneLoaded(bool pSceneLoaded);
		void SetRenderer(Zephyrus::Render::IRenderer* pRenderer);
		void BeginRender();
		void RenderCurrentSceneOnly();
		void EndRender();
		virtual void Unload(); // Unloads scene resources
		virtual void Close(); // Cleans up the scene
		void SaveTo(const std::string& pFilePath);
		void SetPlayerStart(Actor* pPlayerStart);
		void SetFilePath(const std::string& pFilePath);
		void SaveScene();

	public:
		virtual void AddActor(Actor* pActor); // Adds an actor to the scene
		virtual void UpdateAllActors(); // Updates all actors in the scene
		virtual void RemoveActor(Actor* pActor); // Removes an actor from the scene
		virtual void RemoveActorWithID(const std::string& pId);

		Actor* GetActorWithID(const std::string& pID);


		inline Actor* GetPlayerRef() const { return mPlayerRef; }

		inline std::vector<Actor*> GetAllActors() const { return mAllActors; }
		inline std::string GetTitle() const { return mTitle; }
		inline void SetIsSaved(const bool& pSaved) { mSaved = pSaved; }
		inline bool GetIsSaved() const { return mSaved; }
		inline PhysicWorld* GetPhysicWorld() const { return mPhysicWorld; }
		inline PhysicsDebugRenderer* GetPhysicDebugRenderer() const { return mDebugRenderer; }
		inline CameraManager* GetCameraManager() const { return mCameraManager; }
		inline bool GetSceneLoaded() const { return mIsSceneLoaded; }
	public:
		inline Zephyrus::Render::IRenderer* GetRenderer() const { return mRenderer; }
	};
}
