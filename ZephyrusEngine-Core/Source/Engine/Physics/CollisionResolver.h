#pragma once

#include "CollisionInfos.h"
#include "ColliderComponent.h"
#include "Actor.h"

#include <unordered_map>
#include <utility>
#include <algorithm>

class RigidbodyComponent;

// Utility namespace for collision hashing and equality
namespace CollisionUtils {
	struct CollisionPairHash {
		std::size_t operator()(const std::pair<ColliderComponent*, ColliderComponent*>& pair) const {
			ColliderComponent* first = std::min(pair.first, pair.second);
			ColliderComponent* second = std::max(pair.first, pair.second);
			auto h1 = std::hash<ColliderComponent*>{}(first);
			auto h2 = std::hash<ColliderComponent*>{}(second);
			return h1 ^ (h2 << 1);
		}
	};
	struct ActorPairHash {
		std::size_t operator()(const std::pair<Actor*, Actor*>& pair) const {
			Actor* first = std::min(pair.first, pair.second);
			Actor* second = std::max(pair.first, pair.second);
			auto h1 = std::hash<Actor*>{}(first);
			auto h2 = std::hash<Actor*>{}(second);
			return h1 ^ (h2 << 1);
		}
	};
	struct CollisionPairEqual {
		bool operator()(const std::pair<ColliderComponent*, ColliderComponent*>& lhs,
			const std::pair<ColliderComponent*, ColliderComponent*>& rhs) const {
			return (lhs.first == rhs.first && lhs.second == rhs.second) ||
				(lhs.first == rhs.second && lhs.second == rhs.first);
		}
	};

	struct ActorPairEqual {
		bool operator()(const std::pair<Actor*, Actor*>& lhs,
			const std::pair<Actor*, Actor*>& rhs) const {
			return (lhs.first == rhs.first && lhs.second == rhs.second) ||
				(lhs.first == rhs.second && lhs.second == rhs.first);
		}
	};
}

/**
 * @brief Handles the resolution of physics and query collisions between actors and their components.
 * Manages rigidbodies, applies reaction forces, and maintains collision information for the physics engine.
 */
class CollisionResolver
{
public:
	CollisionResolver() = default;
	~CollisionResolver();
	CollisionResolver(const CollisionResolver&) = delete;
	CollisionResolver& operator=(const CollisionResolver&) = delete;

	// Unloads all collision data and clears internal state
	void Unload();

	// Updates all registered rigidbodies (e.g., applies forces, updates positions)
	void UpdateRigidbodies();

	void ResolveCollisions();
	void CalculateQuerryCollisions();
	void CalculatePhysicCollisions();

	// Resolves penetration between two actors by moving them apart along the collision normal
	void ResolvePenetration(Actor* actorA, Actor* actorB, Vector3D normal, float depth);

	// Applies the calculated reaction forces to the rigidbodies
	void ApplyReactionForce();

	// Registers a rigidbody with its owning actor
	void RegisterRigidBody(Actor* pOwner, RigidbodyComponent* pRigidbody);

	// Removes a rigidbody from the resolver
	void RemoveRigidBody(Actor* pOwner, RigidbodyComponent* pRigidbody);

	// Adds a collision information object to the processing queue
	void AddCollisionToQueue(CollisionInfos* pCollisionInfo);

	std::unordered_map<Actor*, RigidbodyComponent*> GetRigidbodies() const { return mRigidbodies; }
private:
	std::unordered_map<std::pair<Actor*, Actor*>, CollisionInfos*,
		CollisionUtils::ActorPairHash, CollisionUtils::ActorPairEqual> mPhysicCollisions;
	std::unordered_map<std::pair<ColliderComponent*, ColliderComponent*>, CollisionInfos*,
		CollisionUtils::CollisionPairHash, CollisionUtils::CollisionPairEqual> mQuerryCollisions;
	std::unordered_map<Actor*, RigidbodyComponent*> mRigidbodies;
	std::unordered_map<Actor*, Vector3D> mCollisionPosition;
	std::unordered_map<RigidbodyComponent*, Vector3D> mReactionForce;
};
