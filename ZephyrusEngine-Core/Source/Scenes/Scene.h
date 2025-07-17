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
public:
	Scene(std::string title = "Scene");
	virtual void Start(IRenderer* renderer); // Initializes the scene with the given renderer
	virtual void Load(); // Loads scene resources
	virtual void Update(); // Updates the scene and its actors
	virtual void Render(); // Renders the scene
	virtual void Unload(); // Unloads scene resources
	virtual void Close(); // Cleans up the scene

public:
	virtual void AddActor(Actor* actor); // Adds an actor to the scene
	virtual void UpdateAllActors(); // Updates all actors in the scene
	virtual void RemoveActor(Actor* actor); // Removes an actor from the scene

public : 
	IRenderer* GetRenderer() const { return mRenderer; }

protected:
	std::string mTitle;
	IRenderer* mRenderer;
	std::vector<Actor*> mAllActors;
	std::vector<Actor*> mPendingActors;
	bool mIsUpdatingActor;
};
