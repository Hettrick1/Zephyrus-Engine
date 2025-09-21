#include "BulletRigidbodyComponent.h"
#include "PhysicWorld.h"

#include "SceneManager.h"
#include "Scene.h"

BulletRigidbodyComponent::BulletRigidbodyComponent(Actor* pOwner)
    : Component(pOwner,"BulletRigidBody"), mType(BodyType::Dynamic), mMass(1.0), mFriction(0.5f), mRestitution(0.5f), mLockAngles(1), mLockAxes(1)
{
}

BulletRigidbodyComponent::~BulletRigidbodyComponent()
{
    if (mRigidBody)
    {
        SceneManager::ActiveScene->GetPhysicWorld()->GetWorld()->removeRigidBody(mRigidBody);
        delete mRigidBody->getMotionState();
        delete mRigidBody;
        mRigidBody = nullptr;
    }
    mShape = nullptr;
    mShapeOwner = nullptr;
    mShapeOwnerId.clear();
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
    if (auto shapeOwner = Serialization::Json::ReadString(pData, "colliderComponentID"))
    {
        if (auto collider = mOwner->GetComponentWithId(*shapeOwner))
        {
            auto colliderComponent = dynamic_cast<BulletColliderComponent*>(collider);
            if (colliderComponent && mShapeOwner != colliderComponent)
            {
                mShape = colliderComponent->GetShape();
                mShapeOwner = colliderComponent;
                mShapeOwnerId = *shapeOwner;
                Initialize(mShapeOwner);
            }
        }
    }
    else
    {
        auto colliders = mOwner->GetAllComponentOfType<BulletColliderComponent>();
        if (!colliders.empty())
        {
            for (auto collider : colliders)
            {
                if (!collider->GetIsQuery() && mShapeOwner != collider)
                {
                    mShape = collider->GetShape();
                    mShapeOwner = collider;
                    mShapeOwnerId = collider->GetId();
                    Initialize(mShapeOwner);
                    return;
                }
            }
        }
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
    if (mShapeOwner)
    {
        pWriter.WriteString("colliderComponentID", mShapeOwnerId);
    }
    Component::EndSerialize(pWriter);
}

void BulletRigidbodyComponent::OnStart()
{
    if (mShapeOwner)
    {
        Initialize(mShapeOwner);
    }
}

void BulletRigidbodyComponent::OnEnd()
{
}

std::vector<PropertyDescriptor> BulletRigidbodyComponent::GetProperties()
{
    return std::vector<PropertyDescriptor>();
}

void BulletRigidbodyComponent::Initialize(BulletColliderComponent* pNewCollider)
{
    mShape = pNewCollider->GetShape();
    mShapeOwner = pNewCollider;
    mShapeOwnerId = mShapeOwner->GetId();

    btVector3 inertia;
    if (mMass != 0.0f)
    {
        mShape->calculateLocalInertia(mMass, inertia);
    }

    auto initialRot = mOwner->GetTransformComponent().GetRotation();
    btQuaternion initialRotation = btQuaternion(initialRot.x, initialRot.y, initialRot.z, initialRot.w);

    auto initialLoc = mOwner->GetTransformComponent().GetPosition();
    btVector3 initialLocation = btVector3(initialLoc.x, initialLoc.y, initialLoc.z);

    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(initialRotation, initialLocation));

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mMass, motionState, mShape, inertia);
    mRigidBody = new btRigidBody(rbInfo);

    mRigidBody->setRestitution(mRestitution);
    mRigidBody->setFriction(mFriction);
    mRigidBody->setLinearFactor(mLockAxes.ToBulletVec3());
    mRigidBody->setAngularFactor(mLockAngles.ToBulletVec3());

    SceneManager::ActiveScene->GetPhysicWorld()->GetWorld()->addRigidBody(mRigidBody);
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
    mShape = nullptr;
    mShapeOwner = nullptr;
    mShapeOwnerId.clear();
}

void BulletRigidbodyComponent::SetMass(float pMass)
{
    if (mMass != pMass)
    {
        mMass = pMass;
        if (mShapeOwner)
        {
            Rebuild(mShapeOwner);
        }
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

void BulletRigidbodyComponent::Rebuild(BulletColliderComponent* pNewCollider)
{
    if (mRigidBody)
    {
        SceneManager::ActiveScene->GetPhysicWorld()->GetWorld()->removeRigidBody(mRigidBody);
        delete mRigidBody->getMotionState();
        delete mRigidBody;
        mRigidBody = nullptr;
    }

    Initialize(pNewCollider);
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

    if (mShapeOwner)
    {
        Rebuild(mShapeOwner);
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
}
