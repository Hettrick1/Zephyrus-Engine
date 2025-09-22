#include "BulletRigidbodyComponent.h"
#include "PhysicWorld.h"

#include "SceneManager.h"
#include "Scene.h"

BulletRigidbodyComponent::BulletRigidbodyComponent(Actor* pOwner)
    : Component(pOwner,"BulletRigidBody"), mType(BodyType::Dynamic), mMass(1.0), mFriction(0.5f), mRestitution(0.5f), mLockAngles(1), mLockAxes(1)
{
    if (mOwner->GetComponentOfType<RigidbodyComponent>())
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
}

BulletRigidbodyComponent::~BulletRigidbodyComponent()
{
    ClearRigidbody();
    delete mCompound;
    mCompound = nullptr;
    for (auto col : mColliders)
    {
        if (!col->GetIsQuery())
        {
            col->CreateColliderWithoutBody();
        }
    }
}

void BulletRigidbodyComponent::Deserialize(const rapidjson::Value& pData)
{
    Component::Deserialize(pData);

    if (auto type = Serialization::Json::ReadString(pData, "bodyType"))
    {
        mType = StringToBodyType(*type);
    }
    if (auto mass = Serialization::Json::ReadFloat(pData, "mass"))
    {
        mMass = *mass;
    }
    if (auto friction = Serialization::Json::ReadFloat(pData, "friction"))
    {
        mFriction = *friction;
    }
    if (auto restitution = Serialization::Json::ReadFloat(pData, "restitution"))
    {
        mRestitution = *restitution;
    }
    if (auto lockAngles = Serialization::Json::ReadVector3D(pData, "lockAngles"))
    {
        mLockAngles = *lockAngles;
    }
    if (auto lockAxes = Serialization::Json::ReadVector3D(pData, "lockAxes"))
    {
        mLockAxes = *lockAxes;
    }
}

void BulletRigidbodyComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
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
    return std::vector<PropertyDescriptor>();
}

void BulletRigidbodyComponent::AddCollider(BulletColliderComponent* collider)
{
    if (!collider || collider->GetIsQuery()) return;

    if (!mCompound)
    {
        mCompound = new btCompoundShape();
    }

    btTransform local;
    local.setRotation(collider->GetRelativeTransform().GetRotation().ToBulletQuat());
    local.setOrigin(collider->GetRelativeTransform().GetTranslation().ToBulletVec3());
    mCompound->addChildShape(local, collider->GetShape());

    mColliders.push_back(collider);

    Rebuild();
}

void BulletRigidbodyComponent::RemoveCollider(BulletColliderComponent* collider)
{
    if (!collider || !mCompound) return;

    mCompound->removeChildShape(collider->GetShape());
    mColliders.erase(std::remove(mColliders.begin(), mColliders.end(), collider), mColliders.end());

    Rebuild();
}

void BulletRigidbodyComponent::ClearRigidbody()
{
    if (mRigidBody)
    {
        SceneManager::ActiveScene->GetPhysicWorld()->GetWorld()->removeRigidBody(mRigidBody);
        delete mRigidBody->getMotionState();
        delete mRigidBody;
        mRigidBody = nullptr;
    }
}

void BulletRigidbodyComponent::SetMass(float pMass)
{
    if (mMass != pMass)
    {
        mMass = pMass;
        Rebuild();
    }
}

void BulletRigidbodyComponent::ApplyForce(const btVector3& force)
{
    if (mRigidBody)
    {
        mRigidBody->applyCentralForce(force);
    }
}

void BulletRigidbodyComponent::ApplyImpulse(const btVector3& impulse)
{
    if (mRigidBody)
    {
        mRigidBody->applyCentralImpulse(impulse);
    }
}

void BulletRigidbodyComponent::ApplyTorque(const btVector3& torque)
{
    if (mRigidBody) 
    {
        mRigidBody->applyTorque(torque);
    }
}

void BulletRigidbodyComponent::ApplyTorqueImpulse(const btVector3& impulse)
{
    if (mRigidBody) 
    {
        mRigidBody->applyTorqueImpulse(impulse);
    }
}

void BulletRigidbodyComponent::SyncTransformFromPhysics()
{
    if (!mRigidBody) return;

    btTransform trans;
    mRigidBody->getMotionState()->getWorldTransform(trans);
    btVector3 pos = trans.getOrigin();
    btQuaternion rot = trans.getRotation();

    mOwner->SetPosition(Vector3D(pos.x(), pos.y(), pos.z()));
    mOwner->SetRotation(Quaternion(rot.x(), rot.y(), rot.z(), rot.w()));
}

void BulletRigidbodyComponent::Rebuild()
{
    ClearRigidbody();

    if (!mCompound || mColliders.empty())
        return;

    btVector3 inertia(0, 0, 0);
    if (mMass > 0.0f)
    {
        mCompound->calculateLocalInertia(mMass, inertia);
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
    mRigidBody->setLinearFactor(mLockAxes.ToBulletVec3());
    mRigidBody->setAngularFactor(mLockAngles.ToBulletVec3());

    SceneManager::ActiveScene->GetPhysicWorld()->GetWorld()->addRigidBody(mRigidBody);
}

void BulletRigidbodyComponent::UpdateColliderShape(BulletColliderComponent* collider, btCollisionShape* oldShape)
{
    if (!mCompound || !collider) return;

    if (oldShape)
    {
        mCompound->removeChildShape(oldShape);
    }

    if (collider->GetShape() && !collider->GetIsQuery())
    {
        btTransform local;
        local.setRotation(collider->GetRelativeTransform().GetRotation().ToBulletQuat());
        local.setOrigin(collider->GetRelativeTransform().GetTranslation().ToBulletVec3());
        mCompound->addChildShape(local, collider->GetShape());
    }

    Rebuild();
}

void BulletRigidbodyComponent::SetType(BodyType pType)
{
    if (mType == pType && mRigidBody) return;

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
