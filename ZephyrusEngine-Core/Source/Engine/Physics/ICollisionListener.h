#pragma once
#include "ColliderComponent.h"
#include "Bullet/BulletColliderComponent.h"
#include "HitResult.h"

class ICollisionListener {
public:
	virtual ~ICollisionListener() = default;
	virtual void OnTriggerEnter(ColliderComponent* collider, HitResult* infos) {};
	virtual void OnTriggerStay(ColliderComponent* collider, HitResult* infos) {};
	virtual void OnTriggerExit(ColliderComponent* collider, HitResult* infos) {};

	virtual void OnTriggerEnter(BulletColliderComponent* collider, HitResult* infos) {};
	virtual void OnTriggerStay(BulletColliderComponent* collider, HitResult* infos) {};
	virtual void OnTriggerExit(BulletColliderComponent* collider, HitResult* infos) {};
};
