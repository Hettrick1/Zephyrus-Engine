#include "BulletRigidbodyComponent.h"
#include "PhysicWorld.h"

#include "SceneManager.h"
#include "Scene.h"

namespace Zephyrus::ActorComponent {
    BulletRigidbodyComponent::BulletRigidbodyComponent(Actor* pOwner)
        : Component(pOwner, "RigidbodyComponent"), mType(BodyType::Dynamic), mMass(1.0), mFriction(0.5f), mRestitution(0.5f), mLockAngles(1), mLockAxes(1)
    {
        pOwner->SetRigidBody(this);
        if (mOwner->GetAllComponentOfType<BulletRigidbodyComponent>().size() > 1)
        {
            return;
        }
        if (!mIsActive)
        {
            return;
        }
        auto colliders = mOwner->GetAllComponentOfType<BulletColliderComponent>();

        if (!colliders.empty())
        {
            for (auto col : colliders)
            {
                if (col->GetIsQuery())
                {
                    continue;
                }
                else
                {
                    col->ClearGhostObject();
                    AddCollider(col);
                }
            }
        }

        Rebuild();
        ForceSyncFromActor();
    }

    BulletRigidbodyComponent::~BulletRigidbodyComponent()
    {
        ClearRigidbody();
        delete mCompound;
        mCompound = nullptr;
        if (!mIsActive)
        {
            return;
        }
        for (auto col : mColliders)
        {
            if (!col->GetIsQuery())
            {
                col->CreateColliderWithoutBody();
            }
        }
    }

    void BulletRigidbodyComponent::Deserialize(Serialization::IDeserializer& pReader)
    {
        Component::Deserialize(pReader);

        if (auto type = pReader.ReadString("bodyType"))
        {
            mType = StringToBodyType(*type);
        }
        if (auto mass = pReader.ReadFloat("mass"))
        {
            mMass = *mass;
        }
        if (auto friction = pReader.ReadFloat("friction"))
        {
            mFriction = *friction;
        }
        if (auto restitution = pReader.ReadFloat("restitution"))
        {
            mRestitution = *restitution;
        }
        if (auto lockAngles = pReader.ReadVector3D("lockAngles"))
        {
            mLockAngles = *lockAngles;
        }
        if (auto lockAxes = pReader.ReadVector3D("lockAxes"))
        {
            mLockAxes = *lockAxes;
        }
        ForceSyncFromActor();
    }

    void BulletRigidbodyComponent::Serialize(Serialization::ISerializer& pWriter)
    {
        Component::BeginSerialize(pWriter);
        pWriter.WriteString("bodyType", BodyTypeToString(mType));
        pWriter.WriteFloat("mass", mMass);
        pWriter.WriteFloat("friction", mFriction);
        pWriter.WriteFloat("restitution", mRestitution);
        pWriter.WriteVector3D("lockAngles", mLockAngles);
        pWriter.WriteVector3D("lockAxes", mLockAxes);
        Component::EndSerialize(pWriter);
    }

    void BulletRigidbodyComponent::OnStart()
    {
        Component::OnStart();
    }

    void BulletRigidbodyComponent::OnEnd()
    {
    }

    std::vector<PropertyDescriptor> BulletRigidbodyComponent::GetProperties()
    {
        SetMass(mMass);
        SetFriction(mFriction);
        SetRestitution(mRestitution);
        LockAngle(mLockAngles);
        LockAxes(mLockAxes);
        return
        {
            //TODO bodytype enum
            {"Mass : ", &mMass, PropertyType::Float},
            {"Friction : ", &mFriction, PropertyType::Float},
            {"Restitution : ", &mRestitution, PropertyType::Float},
            {"Lock Angles : ", &mLockAngles, PropertyType::Vec3},
            {"Lock Axes : ", &mLockAxes, PropertyType::Vec3}
        };
    }

    void BulletRigidbodyComponent::AddCollider(BulletColliderComponent* collider)
    {
        if (!collider || collider->GetIsQuery() || !mIsActive) return;

        if (!mCompound)
        {
            mCompound = new btCompoundShape();
        }

        btTransform local;
        local.setRotation(Zephyrus::Physics::ToBtQuat(collider->GetRelativeTransform().GetRotation()));
        local.setOrigin(Zephyrus::Physics::ToBtVec3(collider->GetRelativeTransform().GetTranslation()));

        auto world = mOwner->GetScene().GetPhysicWorld();
        world->RemoveRigidbody(this);

        mCompound->addChildShape(local, collider->GetShape());
        mColliders.push_back(collider);

        world->AddRigidbody(this);

        Rebuild();
    }

    void BulletRigidbodyComponent::RemoveCollider(BulletColliderComponent* collider)
    {
        if (!collider || !mCompound) return;

        auto world = mOwner->GetScene().GetPhysicWorld();

        world->RemoveRigidbody(this);

        mCompound->removeChildShape(collider->GetShape());
        mColliders.erase(std::remove(mColliders.begin(), mColliders.end(), collider), mColliders.end());

        world->AddRigidbody(this);

        if (mColliders.empty())
        {
            ClearRigidbody();
            delete mCompound;
            mCompound = nullptr;
        }
        else
        {
            Rebuild();
        }
    }

    void BulletRigidbodyComponent::ClearRigidbody()
    {
        if (mRigidBody)
        {
            mOwner->GetScene().GetPhysicWorld()->RemoveRigidbody(this);
            delete mRigidBody->getMotionState();
            delete mRigidBody;
            mRigidBody = nullptr;
        }
    }

    void BulletRigidbodyComponent::SetMass(float pMass)
    {
        if (!mIsActive)
        {
            return;
        }
        if (mMass != pMass)
        {
            mMass = pMass;
            Rebuild();
        }
    }

    void BulletRigidbodyComponent::ApplyForce(const Vector3D& force)
    {
        if (!mIsActive)
        {
            return;
        }
        if (mRigidBody)
        {
            mRigidBody->applyCentralForce(Zephyrus::Physics::ToBtVec3(force));
        }
    }

    void BulletRigidbodyComponent::ApplyImpulse(const Vector3D& impulse)
    {
        if (!mIsActive)
        {
            return;
        }
        if (mRigidBody)
        {
            mRigidBody->applyCentralImpulse(Zephyrus::Physics::ToBtVec3(impulse));
        }
    }

    void BulletRigidbodyComponent::ApplyTorque(const Vector3D& torque)
    {
        if (!mIsActive)
        {
            return;
        }
        if (mRigidBody)
        {
            mRigidBody->applyTorque(Zephyrus::Physics::ToBtVec3(torque));
        }
    }

    void BulletRigidbodyComponent::ApplyTorqueImpulse(const Vector3D& impulse)
    {
        if (!mIsActive)
        {
            return;
        }
        if (mRigidBody)
        {
            mRigidBody->applyTorqueImpulse(Zephyrus::Physics::ToBtVec3(impulse));
        }
    }

    void BulletRigidbodyComponent::SyncTransformFromPhysics()
    {
        if (!mRigidBody || !mIsActive) return;

        btTransform trans;
        mRigidBody->getMotionState()->getWorldTransform(trans);
        btVector3 pos = trans.getOrigin();
        btQuaternion rot = trans.getRotation();

        mOwner->SetPosition(Zephyrus::Physics::FromBtVec3(pos));
        mOwner->SetRotation(Zephyrus::Physics::FromBtQuat(rot));
    }

    void BulletRigidbodyComponent::SyncTransformFromWorld()
    {
        if (!mCompound || !mRigidBody || !mIsActive) return;
        if (mType == BodyType::Kinematic || mType == BodyType::Static)
        {
            btTransform trans;
            trans.setOrigin(Zephyrus::Physics::ToBtVec3(mOwner->GetPosition()));
            trans.setRotation(Zephyrus::Physics::ToBtQuat(mOwner->GetTransformComponent().GetRotation()));

            mRigidBody->setWorldTransform(trans);

            if (mRigidBody->getMotionState())
            {
                mRigidBody->getMotionState()->setWorldTransform(trans);
            }
        }
    }

    void BulletRigidbodyComponent::ForceSyncFromActor()
    {
        if (!mRigidBody) return;

        btTransform trans;
        trans.setOrigin(Zephyrus::Physics::ToBtVec3(mOwner->GetPosition()));
        trans.setRotation(Zephyrus::Physics::ToBtQuat(mOwner->GetTransformComponent().GetRotation()));

        mRigidBody->setWorldTransform(trans);

        if (mRigidBody->getMotionState())
        {
            mRigidBody->getMotionState()->setWorldTransform(trans);
        }

        mRigidBody->activate(true);
    }

    void BulletRigidbodyComponent::UpdateColliderTransform(BulletColliderComponent* collider)
    {
        if (!mCompound || !mIsActive) return;

        int childIndex = -1;

        int n = mCompound->getNumChildShapes();
        for (int i = 0; i < n; ++i)
        {
            if (mCompound->getChildShape(i) == collider->GetShape())
            {
                childIndex = i;
            }
        }
        if (childIndex < 0) return;

        btTransform local;
        local.setRotation(Zephyrus::Physics::ToBtQuat(collider->GetRelativeTransform().GetRotation()));
        local.setOrigin(Zephyrus::Physics::ToBtVec3(collider->GetRelativeTransform().GetTranslation()));

        mCompound->updateChildTransform(childIndex, local, true);
    }

    void BulletRigidbodyComponent::Rebuild()
    {
        if (!mIsActive)
        {
            return;
        }
        ClearRigidbody();

        if (!mCompound || mColliders.empty())
            return;

        btVector3 inertia(0, 0, 0);
        if (mMass > 0.0f)
        {
            mCompound->calculateLocalInertia(mMass, inertia);
            mCompound->setUserPointer(mOwner);
        }

        auto initialRot = mOwner->GetTransformComponent().GetRotation();
        btQuaternion initialRotation(initialRot.x, initialRot.y, initialRot.z, initialRot.w);

        auto initialLoc = mOwner->GetTransformComponent().GetPosition();
        btVector3 initialLocation(initialLoc.x, initialLoc.y, initialLoc.z);

        btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(initialRotation, initialLocation));

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mMass, motionState, mCompound, inertia);
        mRigidBody = new btRigidBody(rbInfo);

        mRigidBody->setRestitution(mRestitution);
        mRigidBody->setFriction(mFriction);
        mRigidBody->setLinearFactor(Zephyrus::Physics::ToBtVec3(mLockAxes));
        mRigidBody->setAngularFactor(Zephyrus::Physics::ToBtVec3(mLockAngles));
        mRigidBody->setUserPointer(mOwner);

        mOwner->GetScene().GetPhysicWorld()->AddRigidbody(this);
    }

    void BulletRigidbodyComponent::UpdateColliderShape(BulletColliderComponent* collider, btCollisionShape* oldShape)
    {
        if (!mCompound || !collider || !mIsActive) return;

        if (oldShape)
        {
            mCompound->removeChildShape(oldShape);
        }

        if (collider->GetShape() && !collider->GetIsQuery())
        {
            btTransform local;
            local.setRotation(Zephyrus::Physics::ToBtQuat(collider->GetRelativeTransform().GetRotation()));
            local.setOrigin(Zephyrus::Physics::ToBtVec3(collider->GetRelativeTransform().GetTranslation()));
            mCompound->addChildShape(local, collider->GetShape());
        }

        Rebuild();
    }

    void BulletRigidbodyComponent::SetType(BodyType pType)
    {
        if (mType == pType || !mIsActive) return;

        mType = pType;

        switch (mType)
        {
        case BodyType::Dynamic:
            mMass = (mMass <= 0.0f) ? 1.0f : mMass;
            break;

        case BodyType::Static:
            mMass = 0.0f;
            break;

        case BodyType::Kinematic:
            mMass = 0.0f;
            break;
        }

        Rebuild();
        if (mRigidBody)
        {
            int flags = mRigidBody->getCollisionFlags();

            if (mType == BodyType::Kinematic)
            {
                flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
                mRigidBody->setCollisionFlags(flags);
                mRigidBody->setActivationState(DISABLE_DEACTIVATION);
            }
            else
            {
                flags &= ~(btCollisionObject::CF_KINEMATIC_OBJECT);
                mRigidBody->setCollisionFlags(flags);
                mRigidBody->setActivationState(ACTIVE_TAG);
            }
        }
    }

    void BulletRigidbodyComponent::SetActive(bool pActive)
    {
        Component::SetActive(pActive);
        if (!mIsActive)
        {
            if (mRigidBody)
            {
                ClearRigidbody();
            }
            return;
        }
        if (mRigidBody)
        {
            if (mOwner->GetSceneContext()->GetActiveScene() && mOwner->GetScene().GetPhysicWorld())
            {
                Rebuild();
            }
            return;
        }
        if (!mCompound && !mColliders.empty())
        {
            mCompound = new btCompoundShape();
            for (auto col : mColliders)
            {
                if (!col || col->GetIsQuery()) continue;
                btCollisionShape* shape = col->GetShape();
                if (!shape) continue;
                btTransform local;
                local.setRotation(Zephyrus::Physics::ToBtQuat(col->GetRelativeTransform().GetRotation()));
                local.setOrigin(Zephyrus::Physics::ToBtVec3(col->GetRelativeTransform().GetTranslation()));
                mCompound->addChildShape(local, shape);
            }
        }
        Rebuild();
    }
}
