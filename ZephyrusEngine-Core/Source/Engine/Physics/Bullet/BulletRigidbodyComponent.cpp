#include "BulletRigidbodyComponent.h"
#include "PhysicWorld.h"

#include "SceneManager.h"
#include "Scene.h"

BulletRigidbodyComponent::BulletRigidbodyComponent(Actor* pOwner)
    : Component(pOwner,"BulletRigidBody"), mType(BodyType::Dynamic), mMass(1.0), mFriction(0.5f), mRestitution(0.5f), mLockAngle(1), mLockAxes(1)
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
}

void BulletRigidbodyComponent::Deserialize(const rapidjson::Value& pData)
{
}

void BulletRigidbodyComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
}

void BulletRigidbodyComponent::OnStart()
{
    if (mShape)
    {
        Initialize(mShape);
    }
}

void BulletRigidbodyComponent::OnEnd()
{
}

std::vector<PropertyDescriptor> BulletRigidbodyComponent::GetProperties()
{
    return std::vector<PropertyDescriptor>();
}

void BulletRigidbodyComponent::Initialize(btCollisionShape* shape)
{
    mShape = shape;

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
    mRigidBody->setAngularFactor(mLockAngle.ToBulletVec3());

    SceneManager::ActiveScene->GetPhysicWorld()->GetWorld()->addRigidBody(mRigidBody);
}

void BulletRigidbodyComponent::SetMass(float pMass)
{
    if (mMass != pMass)
    {
        mMass = pMass;
        if (mShape)
        {
            Rebuild(mShape);
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

void BulletRigidbodyComponent::Rebuild(btCollisionShape* newShape)
{
    if (mRigidBody)
    {
        SceneManager::ActiveScene->GetPhysicWorld()->GetWorld()->removeRigidBody(mRigidBody);
        delete mRigidBody->getMotionState();
        delete mRigidBody;
        mRigidBody = nullptr;
    }

    Initialize(newShape);
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

    if (mShape)
    {
        Rebuild(mShape);
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
