#pragma once
#include "Component.h"
#include <btBulletDynamicsCommon.h>

enum class BodyType
{
    Dynamic,
    Static,
    Kinematic
};

class BulletRigidbodyComponent : public Component
{
private:
    btRigidBody* mRigidBody = nullptr;
    btCollisionShape* mShape = nullptr;
    float mMass;
    float mFriction;
    float mRestitution;

    Vector3D mLockAxes;
    Vector3D mLockAngle;

    BodyType mType;
public:
    BulletRigidbodyComponent(Actor* pOwner);
    ~BulletRigidbodyComponent();

    void Deserialize(const rapidjson::Value& pData) override;
    void Serialize(Serialization::Json::JsonWriter& pWriter) override;

    void OnStart() override;
    void OnEnd() override;

    static Component* Create(Actor* pOwner) { return new BulletRigidbodyComponent(pOwner); }

    std::vector<PropertyDescriptor> GetProperties();

    void Initialize(btCollisionShape* shape);

    void SetMass(float pMass);

    void ApplyForce(const btVector3& force);
    void ApplyImpulse(const btVector3& impulse);
    void ApplyTorque(const btVector3& torque);
    void ApplyTorqueImpulse(const btVector3& impulse);

    btRigidBody* GetRigidBody() const { return mRigidBody; }

    void SyncTransformFromPhysics();

    void Rebuild(btCollisionShape* newShape);

    void SetType(BodyType pType);

    inline void LockAxes(const Vector3D& pLockAxes) 
    { 
        mLockAxes = pLockAxes;
    }
    inline void LockAngle(const Vector3D& pLockAngle) 
    { 
        mLockAngle = pLockAngle;
    }

    inline void SetFriction(float pFriction) 
    { 
        mFriction = pFriction;
    }
    inline void SetRestitution(float pRestitution) 
    { 
        mRestitution = pRestitution;
    }

    inline btVector3 GetAngularVelocity() const 
    { 
        if (mRigidBody)
        {
            return mRigidBody->getAngularVelocity();
        }
        return  btVector3(0, 0, 0); 
    }
    inline void SetAngularVelocity(const btVector3& pAngularVelocity) 
    { 
        if (mRigidBody) 
        {
            mRigidBody->setAngularVelocity(pAngularVelocity);
        }
    }

    inline btVector3 GetVelocity() const
    {
        if (mRigidBody)
        {
            return mRigidBody->getLinearVelocity();
        }
        return btVector3(0, 0, 0);
    }

    inline void SetVelocity(const btVector3& pVelocity)
    {
        if (mRigidBody)
        {
            mRigidBody->setLinearVelocity(pVelocity);
        }
    }

    inline void SetWorldTransform(const btTransform& pTransform) 
    {
        if (mRigidBody) 
        {
            mRigidBody->setWorldTransform(pTransform);
        }
    }
    inline btTransform GetWorldTransform() const 
    {
        if (mRigidBody) 
        {
            return mRigidBody->getWorldTransform();
        }
        return btTransform(); 
    }
};