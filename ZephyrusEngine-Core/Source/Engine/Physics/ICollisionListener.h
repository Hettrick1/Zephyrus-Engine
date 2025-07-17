#pragma once
#include "ColliderComponent.h"
#include "HitResult.h"

class ICollisionListener {
public:
	virtual ~ICollisionListener() = default;
	virtual void OnTriggerEnter(ColliderComponent* collider, HitResult* infos) = 0;
	virtual void OnTriggerStay(ColliderComponent* collider, HitResult* infos) = 0;
	virtual void OnTriggerExit(ColliderComponent* collider, HitResult* infos) = 0;
};
