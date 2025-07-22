#pragma once

#include "Component.h"
#include "Physics/CollisionManager.h"
#include "Physics/HitResult.h"
#include <utility>
#include "Physics/AABB.h"

struct HitResult;
class ICollisionListener;
class IRenderer;
struct ContactManifold;

enum class ColliderType {
	BoxAABB,
	BoxSAT
};

/**
 * @brief Base class for all collider components.
 * Handles collision detection, event notification to listeners, and debug drawing.
 * Can be extended for specific collider shapes and algorithms.
 */
class ColliderComponent : public Component
{
public:
	ColliderComponent() = delete;
	ColliderComponent(Actor* pOwner, int pUpdateOder);
	~ColliderComponent();

	// Returns whether this collider is a query collider (does not physically interact).
	bool GetIsQuerry();
	// Sets whether this collider is a query collider.
	void SetIsQuerry(bool pIsQuerry);
	virtual void Update();
	// Checks collision with another collider and outputs contact information.
	virtual bool CheckCollisionWith(ColliderComponent* pOther, ContactManifold& pInfosOut);
	// Returns the collision position as a pair of points.
	virtual std::pair < Vector3D, Vector3D> GetCollisionPosition() const;
	virtual void DebugDraw(IRenderer& pRenderer);
	// Returns the axis-aligned bounding box of the collider.
	virtual AABB GetAABB() { return AABB(); }
	virtual ColliderType GetColliderType() const = 0;

public:
	// Adds a collision event listener.
	void AddListener(ICollisionListener* pListener);
	// Removes a collision event listener.
	void RemoveListener(ICollisionListener* pListener);

public:
	// Notifies listeners that a collision has started.
	void NotifyListenersStarted(HitResult* pInfos);
	// Notifies listeners that a collision is ongoing.
	void NotifyListenersStay(HitResult* pInfos);
	// Notifies listeners that a collision has ended.
	void NotifyListenersEnded(HitResult* pInfos);

	void SetName(std::string pName) { mName = pName; }
	std::string GetName() const { return mName; }

	Vector3D GetSize() const { return mSize; }

	// Sets whether the collider is active.
	void SetActive(bool pActive);
	inline bool GetIsActive() const { return mIsActive; }

protected :
	bool mIsQuerry = false, mIsActive = true;
	std::vector<ICollisionListener*> mListeners;
	Vector3D mSize;
	std::string mName = "";
};
