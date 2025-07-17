#pragma once

#include "CollisionInfos.h"

class CollisionManager;
class CollisionResolver;
class Actor;
class ColliderComponent;
class RigidbodyComponent;

// this is the physic manager
// handle everything related to physics

class PhysicManager
{
public:
	static PhysicManager& Instance();

	~PhysicManager();

	void Update();

	void Unload();

public: // collision manager
	void RegisterCollider(Actor* pOwner, ColliderComponent* pCollider);
	void RemoveCollider(Actor* pOwner, ColliderComponent* pCollider);
	bool LineTrace(const Vector3D& start, const Vector3D& end, HitResult& outHit, Actor* ignoreActor);

public: // collision resolver
	void RegisterRigidBody(Actor* pOwner, RigidbodyComponent* pRigidbody);
	void RemoveRigidBody(Actor* pOwner, RigidbodyComponent* pRigidbody);
	void AddCollisionToQueue(CollisionInfos* pCollisionInfo);
private:
	PhysicManager();
	CollisionManager* mCollisionManager;
	CollisionResolver* mCollisionResolver;
};

