#pragma once
#include "Component.h"
#include <btBulletDynamicsCommon.h>
#include "BulletColliderComponent.h"

#include <string>

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
    BulletColliderComponent* mShapeOwner = nullptr;
    std::string mShapeOwnerId;
    float mMass;
    float mFriction;
    float mRestitution;

    Vector3D mLockAxes;
    Vector3D mLockAngles;

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

    void Initialize(BulletColliderComponent* pNewCollider);
    void ClearRigidbody();

    void SetMass(float pMass);

    void ApplyForce(const btVector3& force);
    void ApplyImpulse(const btVector3& impulse);
    void ApplyTorque(const btVector3& torque);
    void ApplyTorqueImpulse(const btVector3& impulse);

    btRigidBody* GetRigidBody() const { return mRigidBody; }

    void SyncTransformFromPhysics();

    void Rebuild(BulletColliderComponent* pNewCollider);

    void SetType(BodyType pType);

    inline void LockAxes(const Vector3D& pLockAxes) 
    { 
        mLockAxes = pLockAxes;
    }
    inline void LockAngle(const Vector3D& pLockAngle) 
    { 
        mLockAngles = pLockAngle;
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

    inline BodyType StringToBodyType(const std::string& str) {
        if (str == "active") return BodyType::Dynamic;
        if (str == "paused") return BodyType::Kinematic;
        if (str == "dead") return BodyType::Static;
        return BodyType::Dynamic;
    }

    inline const char* BodyTypeToString(BodyType state) {
        switch (state) {
        case BodyType::Dynamic: return "dynamic";
        case BodyType::Kinematic: return "kinematic";
        case BodyType::Static:   return "static";
        }
        return "dynamic";
    }
};