#pragma once
#include "Component.h"
#include "HitResult.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <unordered_map>

class ICollisionListener;

class BulletColliderComponent : public Component
{
public:
    BulletColliderComponent(Actor* pOwner);
    virtual ~BulletColliderComponent();

    btCollisionShape* GetShape() const { return mShape; }

    virtual void SetIsQuery(bool pIsQuery);
    inline bool GetIsQuery() const { return mIsQuery; }

    void UpdateTrigger();

public:
    // Adds a collision event listener.
    void AddListener(ICollisionListener* pListener);
    // Removes a collision event listener.
    void RemoveListener(ICollisionListener* pListener);

public:
    // Notifies listeners that a collision has started.
    void NotifyListenersStarted(HitResult* pInfos);
    // Notifies listeners that a collision is ongoing.
    void NotifyListenersStay(HitResult* pInfos);
    // Notifies listeners that a collision has ended.
    void NotifyListenersEnded(HitResult* pInfos);


protected:
    btCollisionShape* mShape = nullptr;
    btGhostObject* mGhost = nullptr;
    std::unordered_map<const btCollisionObject*, HitResult> mPreviousOverlaps;
    std::vector<ICollisionListener*> mListeners;
    bool mIsQuery = false;
};