#pragma once
#include "IRenderer.h"
#include <vector>
#include "Assets.h"

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
	IRenderer* mRenderer;
	std::vector<Actor*> mAllActors;
	std::vector<Actor*> mPendingActors;
	bool mIsUpdatingActor;
	Actor* mPlayerRef = nullptr;
	Actor* mPlayerStart = nullptr;
public:
	Scene(std::string pTitle = "Scene");
	virtual void Start(); // Initializes the scene with the given renderer
	virtual void PostStart();
	virtual void Update(); // Updates the scene and its actors
	virtual void Render();
	void SetRenderer(IRenderer* pRenderer);
	void BeginRender();
	void RenderCurrentSceneOnly();
	void EndRender();
	virtual void Unload(); // Unloads scene resources
	virtual void Close(); // Cleans up the scene
	void SaveTo(const std::string& pFilePath);
	void SetPlayerStart(Actor* pPlayerStart);

public:
	virtual void AddActor(Actor* pActor); // Adds an actor to the scene
	virtual void UpdateAllActors(); // Updates all actors in the scene
	virtual void RemoveActor(Actor* pActor); // Removes an actor from the scene

	inline Actor* GetPlayerRef() const { return mPlayerRef; }

	inline std::vector<Actor*> GetAllActors() const { return mAllActors; }

public : 
	inline IRenderer* GetRenderer() const { return mRenderer; }
};
