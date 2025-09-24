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
    btCompoundShape* mCompound = nullptr;
    std::vector<BulletColliderComponent*> mColliders;
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

    void AddCollider(BulletColliderComponent* pCollider);
    void RemoveCollider(BulletColliderComponent* pCollider);

    void ClearRigidbody();

    void SetMass(float pMass);

    void ApplyForce(const Vector3D& pForce);
    void ApplyImpulse(const Vector3D& pImpulse);
    void ApplyTorque(const Vector3D& pTorque);
    void ApplyTorqueImpulse(const Vector3D& pImpulse);

    btRigidBody* GetRigidBody() const { return mRigidBody; }

    void SyncTransformFromPhysics();
    void SyncTransformFromWorld();
    void ForceSyncFromActor();
    void UpdateColliderTransform(BulletColliderComponent* pCollider);

    void Rebuild();
    void UpdateColliderShape(BulletColliderComponent* pCollider, btCollisionShape* pOldShape);

    void SetType(BodyType pType);

    inline void LockAxes(const Vector3D& pLockAxes) 
    { 
        if (mLockAxes != pLockAxes)
        {
            mLockAxes = pLockAxes;
            mRigidBody->setLinearFactor(mLockAxes.ToBulletVec3());
        }
    }
    inline void LockAngle(const Vector3D& pLockAngle) 
    { 
        if (mLockAngles != pLockAngle)
        {
            mLockAngles = pLockAngle;
            mRigidBody->setAngularFactor(mLockAngles.ToBulletVec3());
        }
    }

    inline void SetFriction(float pFriction) 
    { 
        if (mFriction != pFriction)
        {
            mFriction = pFriction;
            mRigidBody->setFriction(mFriction);
        }
    }
    inline void SetRestitution(float pRestitution) 
    { 
        if (mRestitution != pRestitution)
        {
            mRestitution = pRestitution;
            mRigidBody->setRestitution(mRestitution);
        }
    }

    inline Vector3D GetAngularVelocity() const 
    { 
        if (mRigidBody)
        {
            btVector3 vec = mRigidBody->getAngularVelocity();
            return Vector3D(vec.x(), vec.y(), vec.z());
        }
        return  Vector3D(0);
    }
    inline void SetAngularVelocity(const Vector3D& pAngularVelocity) 
    { 
        if (mRigidBody) 
        {
            mRigidBody->setAngularVelocity(pAngularVelocity.ToBulletVec3());
        }
    }

    inline Vector3D GetVelocity() const
    {
        if (mRigidBody)
        {
            btVector3 vec = mRigidBody->getLinearVelocity();
            return Vector3D(vec.x(), vec.y(), vec.z());
        }
        return Vector3D(0);
    }

    inline void SetVelocity(const Vector3D& pVelocity)
    {
        if (mRigidBody)
        {
            mRigidBody->setLinearVelocity(pVelocity.ToBulletVec3());
        }
    }

    inline void SetWorldTransform(const Vector3D& pPosition, const Quaternion& pRotation)
    {
        if (mRigidBody) 
        {
            btTransform transform;
            transform.setOrigin(pPosition.ToBulletVec3());
            transform.setRotation(pRotation.ToBulletQuat());

            mRigidBody->setWorldTransform(transform);
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