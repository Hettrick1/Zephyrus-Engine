#include "CollisionManager.h"
#include "Actor.h"
#include "ICollisionListener.h"
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "PhysicManager.h"
#include "Scene.h"
#include "ContactManifold.h"
#include <utility>
#include <algorithm>

CollisionManager::CollisionManager()
    : mCollisionNormal(0), mCollisionDepth(0)
{
}

CollisionManager::~CollisionManager()
{

    for (auto& pair : mColliders) 
    {
        for (auto* collider : pair.second) 
        {
            if (collider != nullptr)
            {
                delete collider;
            }
            collider = nullptr;
        }
    }
    mColliders.clear();
    mCurrentCollisions.clear();
}

void CollisionManager::Unload()
{
    mColliders.clear();
    mCurrentCollisions.clear();
}

void CollisionManager::RegisterCollider(Actor* pOwner, ColliderComponent* pCollider)
{
    if (pOwner && pCollider) {
        mColliders[pOwner].push_back(pCollider);
    }
}

void CollisionManager::RemoveCollider(Actor* pOwner, ColliderComponent* pCollider)
{
    auto it = mColliders.find(pOwner);
    if (it != mColliders.end())
    {
        std::vector<ColliderComponent*>& colliders = it->second;

        auto colliderIt = std::find(colliders.begin(), colliders.end(), pCollider);
        if (colliderIt != colliders.end())
        {
            colliders.erase(colliderIt);
        }

        if (colliders.empty())
        {
            mColliders.erase(it);
        }
    }
}

void CollisionManager::UpdateColliders()
{
    for (auto it = mColliders.begin(); it != mColliders.end(); it++)
    {
        for (auto& col : it->second) 
        {
            col->Update();
        }
    }
}

void CollisionManager::CheckCollisions()
{
    if (mColliders.empty()) {
        return;
    }

    std::unordered_map<ColliderComponent*, std::unordered_set<ColliderComponent*>> newCollisions;

    // Get active actor
    std::vector<Actor*> activeActors;
    for (auto& pair : mColliders) {
        if (pair.first->GetState() == ActorState::Active) {
            activeActors.push_back(pair.first);
        }
    }
    for (auto it = mCurrentCollisions.begin(); it != mCurrentCollisions.end(); ) {
        ColliderComponent* collider = it->first;
        Actor* owner = collider->GetOwner();

        if (owner->GetState() != ActorState::Active) {
            it = mCurrentCollisions.erase(it);
        }
        else {
            for (auto otherIt = it->second.begin(); otherIt != it->second.end(); ) {
                if ((*otherIt)->GetOwner()->GetState() != ActorState::Active) {
                    otherIt = it->second.erase(otherIt);
                }
                else {
                    ++otherIt;
                }
            }
            ++it;
        }
    }

    // Begin and stay collision handling
    for (size_t i = 0; i < activeActors.size(); ++i) {
        for (size_t j = i + 1; j < activeActors.size(); ++j) {
            Actor* actor1 = activeActors[i];
            Actor* actor2 = activeActors[j];

            auto& colliders1 = mColliders[actor1];
            auto& colliders2 = mColliders[actor2];

            for (auto* collider1 : colliders1) {
                for (auto* collider2 : colliders2) {
                    if (!collider1->GetIsActive() || !collider2->GetIsActive())
                    {
                        continue;
                    }
                    ContactManifold manifold;
                    if (collider1->CheckCollisionWith(collider2, manifold)) {
                        bool isNewCollision1 = mCurrentCollisions[collider1].find(collider2) == mCurrentCollisions[collider1].end();
                        bool isNewCollision2 = mCurrentCollisions[collider2].find(collider1) == mCurrentCollisions[collider2].end();

                        std::pair < Vector3D, Vector3D> collisionPos = collider1->GetCollisionPosition();

                        std::pair<Actor*, Actor*> actorPair;
                        actorPair.first = collider1->GetOwner();
                        actorPair.second = collider2->GetOwner();

                        std::pair<ColliderComponent*, ColliderComponent*> colliderPair;
                        colliderPair.first = collider1;
                        colliderPair.second = collider2;

                        if (isNewCollision1 || isNewCollision2) { //enter
                            CollisionInfos* infos = new CollisionInfos(actorPair, colliderPair, CollisionType::Enter, manifold.normal, manifold.penetrationDepth, collisionPos);
                            PhysicManager::Instance().AddCollisionToQueue(infos);
                        }
                        else { // stays
                            CollisionInfos* infos = new CollisionInfos(actorPair, colliderPair, CollisionType::Stay, manifold.normal, manifold.penetrationDepth, collisionPos);
                            PhysicManager::Instance().AddCollisionToQueue(infos);
                        }
                        newCollisions[collider1].insert(collider2);
                        newCollisions[collider2].insert(collider1);
                    }
                }
            }
        }
    }
    // End of collision handling
    std::unordered_set<std::pair<ColliderComponent*, ColliderComponent*>, CollisionPairHash> processedCollisions;
    for (auto& pair : mCurrentCollisions) {
        ColliderComponent* collider = pair.first;
        for (auto* otherCollider : pair.second) {
            if (newCollisions[collider].find(otherCollider) == newCollisions[collider].end()) {
                auto collisionPair = std::minmax(collider, otherCollider);
                if (processedCollisions.find(collisionPair) == processedCollisions.end()) {
                    processedCollisions.insert(collisionPair);

                    std::pair<Actor*, Actor*> actorPair;
                    actorPair.first = collisionPair.first->GetOwner(); 
                    actorPair.second = collisionPair.second->GetOwner();

                    std::pair<Vector3D, Vector3D> col;
                    col.first = 0;
                    col.second = 0;

                    // exit
                    CollisionInfos* infos = new CollisionInfos(actorPair, collisionPair, CollisionType::Exit, mCollisionNormal, 0, col);
                    PhysicManager::Instance().AddCollisionToQueue(infos);
                }
            }
        }
    }
    mCurrentCollisions = newCollisions;
}

void CollisionManager::CalculateNormal(ColliderComponent* collider1, ColliderComponent* collider2)
{
    Vector3D pos1 = collider1->GetOwner()->GetTransformComponent().GetPosition();
    Vector3D pos2 = collider2->GetOwner()->GetTransformComponent().GetPosition();

    Vector3D normal;

    Vector3D halfSize1 = collider1->GetSize();
    Vector3D halfSize2 = collider2->GetSize();

    Vector3D min1 = pos1 - halfSize1;
    Vector3D max1 = pos1 + halfSize1;
    Vector3D min2 = pos2 - halfSize2;
    Vector3D max2 = pos2 + halfSize2;

    normal = (pos2 - pos1);
    normal = Vector3D(normal.x, normal.y, 0);

    float overlapX = std::min(max1.x, max2.x) - std::max(min1.x, min2.x);
    float overlapY = std::min(max1.y, max2.y) - std::max(min1.y, min2.y);
    float overlapZ = std::min(max1.z, max2.z) - std::max(min1.z, min2.z);

    float minOverlap = std::min({ overlapX, overlapY, overlapZ });

    mCollisionNormal = normal;
    mCollisionDepth = minOverlap;
}

bool CollisionManager::LineTrace(const Vector3D& start, const Vector3D& end, HitResult& outHit, Actor* ignoreActor)
{
    bool bHasHit = false;
    float closestHit = FLT_MAX;
    Vector3D rayDir = (end - start);

    for (const auto& pair : mColliders)
    {
        for (auto* collider : pair.second)
        {
            if (!collider)
            {
                continue;
            }
            if (collider->GetOwner() == ignoreActor)
            {
                continue;
            }
            if (!collider->GetIsActive())
            {
                continue;
            }

            AABB aabb = collider->GetAABB();
            float hitDistance;

            if (aabb.RayIntersects(start, end, hitDistance) && hitDistance < closestHit)
            {
                closestHit = hitDistance;
                outHit.HasHit = true;
                outHit.HitPoint = start + rayDir * hitDistance;
                outHit.Distance = hitDistance;
                outHit.HitActor = pair.first;
                outHit.HitCollider = collider;
                bHasHit = true;
            }
        }
    }

    return bHasHit;
}

