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

    void AddCollider(BulletColliderComponent* collider);
    void RemoveCollider(BulletColliderComponent* collider);

    void ClearRigidbody();

    void SetMass(float pMass);

    void ApplyForce(const Vector3D& force);
    void ApplyImpulse(const Vector3D& impulse);
    void ApplyTorque(const Vector3D& torque);
    void ApplyTorqueImpulse(const Vector3D& impulse);

    btRigidBody* GetRigidBody() const { return mRigidBody; }

    void SyncTransformFromPhysics();
    void UpdateColliderTransform(BulletColliderComponent* collider);

    void Rebuild();
    void UpdateColliderShape(BulletColliderComponent* collider, btCollisionShape* oldShape);

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
    inline void SetAngularVelocity(const Vector3D& pAngularVelocity) 
    { 
        if (mRigidBody) 
        {
            mRigidBody->setAngularVelocity(pAngularVelocity.ToBulletVec3());
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