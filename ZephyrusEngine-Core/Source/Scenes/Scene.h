#pragma once
#include "IRenderer.h"
#include <vector>
#include <unordered_map>
#include "Assets.h"
#include "Physics/Bullet/PhysicWorld.h"
#include "Physics/Bullet/PhysicsDebugRenderer.h"

class Actor;

/**
 * @brief Represents a scene containing actors and manages their lifecycle.
 * Handles initialization, loading, updating, rendering, unloading, and closing of the scene.
 * Also manages the addition and removal of actors.
 */
class Scene
{
protected:
	std::string mTitle;
	std::string mFilePath;
	std::string mPlayerPrefabName;
	IRenderer* mRenderer;
	std::unordered_map<std::string, Actor*> mActors;
	std::vector<Actor*> mAllActors;
	std::vector<Actor*> mPendingActors;
	bool mIsUpdatingActor;
	Actor* mPlayerRef = nullptr;
	Actor* mPlayerStart = nullptr;
	bool mSaved = true;
	PhysicWorld* mPhysicWorld = nullptr;
	PhysicsDebugRenderer* mDebugRenderer = nullptr;
public:
	Scene(std::string pTitle = "Scene");
	virtual void Start(); // Initializes the scene with the given renderer
	virtual void PostStart();
	virtual void Update(float pDetltaTime); // Updates the scene and its actors
	virtual void Render();
	void SetRenderer(IRenderer* pRenderer);
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
public : 
	inline IRenderer* GetRenderer() const { return mRenderer; }
};
