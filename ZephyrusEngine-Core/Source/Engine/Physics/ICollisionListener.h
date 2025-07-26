#pragma once
#include "ColliderComponent.h"
#include "HitResult.h"

class ICollisionListener {
public:
	virtual ~ICollisionListener() = default;
	virtual void OnTriggerEnter(ColliderComponent* collider, HitResult* infos) {};
	virtual void OnTriggerStay(ColliderComponent* collider, HitResult* infos) {};
	virtual void OnTriggerExit(ColliderComponent* collider, HitResult* infos) {};
};
