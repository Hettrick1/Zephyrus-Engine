#pragma once
#include "Bullet/BulletColliderComponent.h"
#include "HitResult.h"

using Zephyrus::ActorComponent::BulletColliderComponent;

namespace Zephyrus::Physics {
	class ICollisionListener {
	public:
		virtual ~ICollisionListener() = default;
		virtual void OnTriggerEnter(BulletColliderComponent* collider, HitResult* infos) {};
		virtual void OnTriggerStay(BulletColliderComponent* collider, HitResult* infos) {};
		virtual void OnTriggerExit(BulletColliderComponent* collider, HitResult* infos) {};
	};
}
