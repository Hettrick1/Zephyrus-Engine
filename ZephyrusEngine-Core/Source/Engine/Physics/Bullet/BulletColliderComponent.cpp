#include "BulletColliderComponent.h"
#include "SceneManager.h"
#include "ICollisionListener.h"

using Zephyrus::Scenes::SceneManager;

namespace Zephyrus::ActorComponent {
    BulletColliderComponent::BulletColliderComponent(Actor* pOwner, const std::string& pName)
        : Component(pOwner, pName)
    {
        if (!mIsActive)
        {
            return;
        }
        SceneManager::ActiveScene->GetPhysicWorld()->AddCollider(this);
    }

    BulletColliderComponent::~BulletColliderComponent()
    {
        if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
        {
            rb->RemoveCollider(this);
        }
        if (mShape)
        {
            delete mShape;
            mShape = nullptr;
        }
        if (mGhost)
        {
            SceneManager::ActiveScene->GetPhysicWorld()->RemoveGhostObject(mGhost);
            delete mGhost;
            mGhost = nullptr;
        }
        SceneManager::ActiveScene->GetPhysicWorld()->RemoveCollider(this);
    }

    void BulletColliderComponent::Deserialize(const rapidjson::Value& pData)
    {
        Component::Deserialize(pData);
        if (auto query = Serialization::Json::ReadBool(pData, "isQuery"))
        {
            SetIsQuery(*query);
        }
        if (auto ignoreSelf = Serialization::Json::ReadBool(pData, "ignoreSelf"))
        {
            mIgnoreSelf = *ignoreSelf;
        }
    }

    void BulletColliderComponent::BeginSerialize(Serialization::Json::JsonWriter& pWriter)
    {
        Component::BeginSerialize(pWriter);
    }

    void BulletColliderComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
    {
        pWriter.WriteBool("isQuery", mIsQuery);
        pWriter.WriteBool("ignoreSelf", mIgnoreSelf);
    }

    void BulletColliderComponent::EndSerialize(Serialization::Json::JsonWriter& pWriter)
    {
        Component::EndSerialize(pWriter);
    }

    void BulletColliderComponent::CreateColliderWithoutBody()
    {
        if (mIsQuery || mOwner->GetState() != ActorState::Active)
        {
            return;
        }
        auto world = SceneManager::ActiveScene->GetPhysicWorld();
        if (mGhost)
        {
            world->RemoveGhostObject(mGhost);
            delete mGhost;
            mGhost = nullptr;
        }
        mGhost = new btGhostObject();
        mGhost->setUserPointer(mOwner);
        mGhost->setCollisionShape(mShape);

        btTransform t;
        t.setOrigin((mOwner->GetPosition() + mRelativePosition).ToBulletVec3());
        auto worldRot = mOwner->GetTransformComponent().GetRotation() * mRelativeRotation;
        t.setRotation(worldRot.ToBulletQuat());
        mGhost->setWorldTransform(t);

        world->AddGhostObject(mGhost);
    }

    void BulletColliderComponent::ClearGhostObject()
    {
        auto world = SceneManager::ActiveScene->GetPhysicWorld();
        if (mGhost)
        {
            world->RemoveGhostObject(mGhost);
            delete mGhost;
            mGhost = nullptr;
        }
    }

    void BulletColliderComponent::SetIsQuery(bool pIsQuery)
    {
        if (mIsQuery == pIsQuery || mOwner->GetState() != ActorState::Active) return;

        mIsQuery = pIsQuery;

        auto world = SceneManager::ActiveScene->GetPhysicWorld();
        auto rb = mOwner->GetRigidBody();

        if (mIsQuery)
        {
            if (rb)
            {
                rb->RemoveCollider(this);
            }

            if (mGhost)
            {
                world->RemoveGhostObject(mGhost);
                delete mGhost;
                mGhost = nullptr;
            }
            mGhost = new btGhostObject();
            mGhost->setUserPointer(mOwner);
            mGhost->setCollisionShape(mShape);
            mGhost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

            btTransform t;
            auto actorRot = mOwner->GetTransformComponent().GetRotation();
            t.setRotation(btQuaternion(actorRot.x, actorRot.y, actorRot.z, actorRot.w));
            t.setOrigin(mOwner->GetPosition().ToBulletVec3());
            mGhost->setWorldTransform(t);

            world->AddGhostObject(mGhost);
            return;
        }
        else if (rb)
        {
            if (mGhost)
            {
                world->RemoveGhostObject(mGhost);
                delete mGhost;
                mGhost = nullptr;
            }
            rb->AddCollider(this);
            return;
        }
        else if (mGhost)
        {
            world->RemoveGhostObject(mGhost);
            delete mGhost;
            mGhost = nullptr;
        }
        CreateColliderWithoutBody();
    }

    void BulletColliderComponent::UpdateTrigger()
    {
        if (!mIsQuery || !mGhost || mOwner->GetState() != ActorState::Active) return;

        std::unordered_map<const btCollisionObject*, HitResult> currentOverlaps;

        int num = mGhost->getNumOverlappingObjects();
        for (int i = 0; i < num; ++i)
        {
            const btCollisionObject* obj = mGhost->getOverlappingObject(i);
            auto hitActor = static_cast<Actor*>(obj->getUserPointer());

            if (mIgnoreSelf)
            {
                if (hitActor == mOwner)
                {
                    continue;
                }
            }

            HitResult result;
            result.HasHit = true;
            result.HitActor = hitActor;
            result.HitCollider = nullptr;
            currentOverlaps[obj] = result;

            if (mPreviousOverlaps.find(obj) == mPreviousOverlaps.end())
            {
                NotifyListenersStarted(&result);
                //DEBUG FOR NOW
                ZP_CORE_INFO("Enter" + mOwner->GetPrefabName());
            }
            else
            {
                NotifyListenersStay(&result);
                //DEBUG FOR NOW
                ZP_CORE_INFO("Stay" + mOwner->GetPrefabName() + " Collide " + result.HitActor->GetPrefabName() + (result.HitActor->HasTag("Ground") ? " ground " : " false "));
            }
        }

        for (auto& prev : mPreviousOverlaps)
        {
            if (currentOverlaps.find(prev.first) == currentOverlaps.end())
            {
                NotifyListenersEnded(&prev.second);
                //DEBUG FOR NOW
                ZP_CORE_ERROR("Exit" + mOwner->GetPrefabName());
            }
        }

        mPreviousOverlaps = std::move(currentOverlaps);
    }

    void BulletColliderComponent::UpdateWorldTransform()
    {
        if (mGhost)
        {
            btTransform t;
            t.setOrigin((mOwner->GetPosition() + mRelativePosition).ToBulletVec3());
            auto worldRot = mOwner->GetTransformComponent().GetRotation() * mRelativeRotation;
            t.setRotation(btQuaternion(worldRot.x, worldRot.y, worldRot.z, worldRot.w));
            mGhost->setWorldTransform(t);
        }
        else if (auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>())
        {
            rb->UpdateColliderTransform(this);
        }
    }

    void BulletColliderComponent::RebuildCollider()
    {
        if (mOwner->GetState() != ActorState::Active)
        {
            return;
        }
        auto world = SceneManager::ActiveScene->GetPhysicWorld();
        auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>();

        if (mIsQuery)
        {
            if (rb)
            {
                rb->RemoveCollider(this);
            }

            if (mGhost)
            {
                world->RemoveGhostObject(mGhost);
                delete mGhost;
                mGhost = nullptr;
            }
            mGhost = new btGhostObject();
            mGhost->setUserPointer(mOwner);
            mGhost->setCollisionShape(mShape);
            mGhost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

            btTransform t;
            auto actorRot = mOwner->GetTransformComponent().GetRotation();
            t.setRotation(btQuaternion(actorRot.x, actorRot.y, actorRot.z, actorRot.w));
            t.setOrigin(mOwner->GetPosition().ToBulletVec3());
            mGhost->setWorldTransform(t);

            world->AddGhostObject(mGhost);
            return;
        }
        else if (rb)
        {
            if (mGhost)
            {
                world->RemoveGhostObject(mGhost);
                delete mGhost;
                mGhost = nullptr;
            }
            rb->AddCollider(this);
            return;
        }
        else if (mGhost)
        {
            world->RemoveGhostObject(mGhost);
            delete mGhost;
            mGhost = nullptr;
        }
        CreateColliderWithoutBody();
    }

    void BulletColliderComponent::OnStart()
    {
        Component::OnStart();
    }

    void BulletColliderComponent::OnEnd()
    {
        Component::OnEnd();
    }

    void BulletColliderComponent::SetActive(bool pActive)
    {
        Component::SetActive(pActive);

        auto world = SceneManager::ActiveScene->GetPhysicWorld();
        auto rb = mOwner->GetComponentOfType<BulletRigidbodyComponent>();

        if (mIsActive)
        {
            if (mIsQuery)
            {
                if (!mGhost)
                {
                    mGhost = new btGhostObject();
                    mGhost->setUserPointer(mOwner);
                    mGhost->setCollisionShape(mShape);
                    mGhost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

                    btTransform t;
                    auto actorRot = mOwner->GetTransformComponent().GetRotation();
                    t.setRotation(btQuaternion(actorRot.x, actorRot.y, actorRot.z, actorRot.w));
                    t.setOrigin(mOwner->GetPosition().ToBulletVec3());
                    mGhost->setWorldTransform(t);

                    world->AddGhostObject(mGhost);
                }
            }
            else if (rb)
            {
                rb->AddCollider(this);
            }
            else
            {
                CreateColliderWithoutBody();
            }

            world->AddCollider(this);
        }
        else
        {
            if (mGhost)
            {
                world->RemoveGhostObject(mGhost);
                delete mGhost;
                mGhost = nullptr;
            }

            if (rb)
            {
                rb->RemoveCollider(this);
            }

            world->RemoveCollider(this);
        }
    }

    void BulletColliderComponent::AddListener(Zephyrus::Physics::ICollisionListener* pListener)
    {
        mListeners.push_back(pListener);
    }

    void BulletColliderComponent::RemoveListener(Zephyrus::Physics::ICollisionListener* pListener)
    {
        mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), pListener), mListeners.end());
    }

    void BulletColliderComponent::NotifyListenersStarted(HitResult* pInfos)
    {
        if (mListeners.size() > 0) {
            for (auto& listener : mListeners) {
                if (listener != nullptr)
                {
                    listener->OnTriggerEnter(this, pInfos);
                }
            }
        }
    }

    void BulletColliderComponent::NotifyListenersStay(HitResult* pInfos)
    {
        if (mListeners.size() > 0) {
            for (auto& listener : mListeners) {
                if (listener != nullptr)
                {
                    listener->OnTriggerStay(this, pInfos);
                }
            }
        }
    }

    void BulletColliderComponent::NotifyListenersEnded(HitResult* pInfos)
    {
        if (mListeners.size() > 0) {
            for (auto& listener : mListeners) {
                if (listener != nullptr)
                {
                    listener->OnTriggerExit(this, pInfos);
                }
            }
        }
    }
}
