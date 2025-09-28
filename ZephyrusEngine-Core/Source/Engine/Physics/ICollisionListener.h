#pragma once
#include "Bullet/BulletColliderComponent.h"
#include "HitResult.h"

class ICollisionListener {
public:
	virtual ~ICollisionListener() = default;
	virtual void OnTriggerEnter(BulletColliderComponent* collider, HitResult* infos) {};
	virtual void OnTriggerStay(BulletColliderComponent* collider, HitResult* infos) {};
	virtual void OnTriggerExit(BulletColliderComponent* collider, HitResult* infos) {};
};
