#pragma once

#include "Component.h"
#include "Vector3D.h"
#include "Maths.h"

class Actor;
class ColliderComponent;

/**
 * @brief RigidbodyComponent handles the physics simulation for an Actor, including velocity, mass, gravity, friction, and collision response.
 */
class RigidbodyComponent : public Component
{
public:
	RigidbodyComponent(Actor* pOwner, int pUpdateOrder = 100);
	RigidbodyComponent() = delete;
	RigidbodyComponent(const RigidbodyComponent&) = delete;
	RigidbodyComponent& operator=(const RigidbodyComponent&) = delete;
public :
	void Update(); // Updates the physics state of the rigidbody
	void ApplyForce(Vector3D pForce); // Applies a continuous force to the rigidbody
	void AddImpulse(Vector3D pImpulse); // Applies an instant impulse to the rigidbody
	void ResolveCollision(Vector3D pResolveForce); // Resolves a collision by applying a force
	void OnCollisionEnter(ColliderComponent* otherCollider); // Called when a collision starts
	void OnCollisionStay(ColliderComponent* otherCollider); // Called while a collision persists
	void OnCollisionExit(ColliderComponent* otherCollider); // Called when a collision ends
public:
	void SetVelocity(Vector3D pVelocity);
	void SetMass(float pMass);
	void SetFriction(float pFriction);
	void SetUseGravity(bool pUseGravity);
	void SetIsGrounded(bool pIsGrounded);
	void SetGravity(float pGravity);
	Vector3D GetVelocity() const { return mVelocity; }
	float GetMass() const { return mMass; }
	bool GetIsGrounded() const { return mIsGrounded; }
	void SetBounciness(float bounce) { bounce <= 1 ? mBounciness = bounce : mBounciness = 1.0f; }
	float GetBounciness() const { return mBounciness; }

	void SetIsStatic(bool isStatic); // Sets whether the rigidbody is static (non-movable)
	bool IsStatic() const { return mIsStatic; }

private:
	Vector3D mVelocity;
	Vector3D mAcceleration;
	float mMass;
	float mFriction;
	bool mUseGravity;
	float mGravity;
	float mBounciness;
	bool mIsGrounded, mIsStatic;
};
