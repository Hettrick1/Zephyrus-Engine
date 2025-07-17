#pragma once

#include <utility>
#include "Actor.h"
#include "ColliderComponent.h"

enum class CollisionType
{
	Enter,
	Stay,
	Exit
};

struct CollisionInfos
{
	CollisionInfos(
		std::pair<Actor*, Actor*> pActorPair, 
		std::pair<ColliderComponent*, ColliderComponent*> pColliderPair,
		CollisionType pType,
		Vector3D pNormal,
		float pDepth,
		std::pair<Vector3D, Vector3D> pPositionPair
	)
	{
		actorPair = pActorPair;
		colliderPair = pColliderPair;
		if (actorPair.first->GetRigidBody() != nullptr && actorPair.second->GetRigidBody() != nullptr)
		{
			velocityPair.first = actorPair.first->GetRigidBody()->GetVelocity();
			velocityPair.second = actorPair.second->GetRigidBody()->GetVelocity();
		}
		else 
		{
			velocityPair.first = Vector3D(0);
			velocityPair.second = Vector3D(0);
		}
		positionPair = pPositionPair;
		type = pType;
		normal = pNormal;
		depth = pDepth;
	}
	std::pair<Actor*, Actor*> actorPair;
	std::pair<ColliderComponent*, ColliderComponent*> colliderPair;
	std::pair<Vector3D, Vector3D> velocityPair;
	std::pair<Vector3D, Vector3D> positionPair;
	CollisionType type;
	Vector3D normal;
	float depth;
};