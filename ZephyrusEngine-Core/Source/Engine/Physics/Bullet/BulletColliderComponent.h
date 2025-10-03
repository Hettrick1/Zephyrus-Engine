#pragma once
#include "Component.h"
#include "HitResult.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <unordered_map>

namespace Zephyrus::Physics 
{ 
    class ICollisionListener; 
    struct HitResult;
}

using Zephyrus::Physics::HitResult;

namespace Zephyrus::ActorComponent {
    class BulletColliderComponent : public Component
    {
    protected:
        btCollisionShape* mShape = nullptr;
        btGhostObject* mGhost = nullptr;
        std::unordered_map<const btCollisionObject*, HitResult> mPreviousOverlaps;
        std::vector<Zephyrus::Physics::ICollisionListener*> mListeners;
        bool mIsQuery = false;
        bool mIgnoreSelf = true;
    public:
        BulletColliderComponent(Actor* pOwner, const std::string& pName = "");
        virtual ~BulletColliderComponent();

        virtual void Deserialize(const rapidjson::Value& pData) override;
        void BeginSerialize(Serialization::Json::JsonWriter& pWriter) override;
        virtual void Serialize(Serialization::Json::JsonWriter& pWriter) override;
        void EndSerialize(Serialization::Json::JsonWriter& pWriter) override;

        void OnStart() override;
        void OnEnd() override;

        void SetActive(bool pActive) override;

        btCollisionShape* GetShape() const { return mShape; }

        void CreateColliderWithoutBody();
        void ClearGhostObject();

        virtual void SetIsQuery(bool pIsQuery);
        inline bool GetIsQuery() const { return mIsQuery; }

        void UpdateTrigger();
        void UpdateWorldTransform();

        void RebuildCollider();

    public:
        // Adds a collision event listener.
        void AddListener(Zephyrus::Physics::ICollisionListener* pListener);
        // Removes a collision event listener.
        void RemoveListener(Zephyrus::Physics::ICollisionListener* pListener);

    public:
        // Notifies listeners that a collision has started.
        void NotifyListenersStarted(HitResult* pInfos);
        // Notifies listeners that a collision is ongoing.
        void NotifyListenersStay(HitResult* pInfos);
        // Notifies listeners that a collision has ended.
        void NotifyListenersEnded(HitResult* pInfos);
    };
}