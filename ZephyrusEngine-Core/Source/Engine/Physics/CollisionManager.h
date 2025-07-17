#pragma once
#include "Log.h"
#include "Actor.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "HitResult.h"
#include "AABB.h"

// Collision Manager Class to handle all the collision check and to send all the event related to collisons

class ColliderComponent;
class ICollisionListener;
class RigidbodyComponent;

class CollisionManager
{
public:
    CollisionManager();
    ~CollisionManager();

    void Unload();

    void RegisterCollider(Actor* pOwner, ColliderComponent* pCollider);
    void RemoveCollider(Actor* pOwner, ColliderComponent* pCollider);
    void UpdateColliders();
    void CheckCollisions();
    void CalculateNormal(ColliderComponent* collider1, ColliderComponent* collider2);

    bool LineTrace(const Vector3D& start, const Vector3D& end, HitResult& outHit, Actor* ignoreActor);
private:
    std::unordered_map<Actor*, std::vector<ColliderComponent*>> mColliders;
    std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> mCurrentCollisions;

    Vector3D mCollisionNormal;
    float mCollisionDepth;
};

struct CollisionPairHash { // hash to handle collision pair, to send only one OnTriggerEnd event
    std::size_t operator()(const std::pair<ColliderComponent*, ColliderComponent*>& pair) const {
        auto h1 = std::hash<ColliderComponent*>{}(pair.first);
        auto h2 = std::hash<ColliderComponent*>{}(pair.second);
        return h1 ^ (h2 << 1); // XOR to minimize hash collisions
    }
};
