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
private:
	Vector3D mVelocity;
	Vector3D mAcceleration;
	float mMass = 0.0;
	float mFriction = 0.0;
	bool mUseGravity = false;
	float mGravity = 0.0;
	float mBounciness = 0.0;
	bool mIsGrounded = false, mIsStatic = false;
public:
	RigidbodyComponent(Actor* pOwner, int pUpdateOrder = 100);
	RigidbodyComponent() = delete;
	RigidbodyComponent(const RigidbodyComponent&) = delete;
	RigidbodyComponent& operator=(const RigidbodyComponent&) = delete;

	void Update(); // Updates the physics state of the rigidbody
	void ApplyForce(Vector3D pForce); // Applies a continuous force to the rigidbody
	void AddImpulse(Vector3D pImpulse); // Applies an instant impulse to the rigidbody
	void ResolveCollision(Vector3D pResolveForce); // Resolves a collision by applying a force
	void OnCollisionEnter(ColliderComponent* pOtherCollider); // Called when a collision starts
	void OnCollisionStay(ColliderComponent* pOtherCollider); // Called while a collision persists
	void OnCollisionExit(ColliderComponent* pOtherCollider); // Called when a collision ends

	void SetVelocity(Vector3D pVelocity);
	void SetMass(float pMass);
	void SetFriction(float pFriction);
	void SetUseGravity(bool pUseGravity);
	void SetIsGrounded(bool pIsGrounded);
	void SetGravity(float pGravity);

	void SetIsStatic(bool pIsStatic); // Sets whether the rigidbody is static (non-movable)

	Vector3D GetVelocity() const { return mVelocity; }
	float GetMass() const { return mMass; }
	bool GetIsGrounded() const { return mIsGrounded; }
	void SetBounciness(float pBounce) { pBounce <= 1 ? mBounciness = pBounce : mBounciness = 1.0f; }
	float GetBounciness() const { return mBounciness; }
	bool IsStatic() const { return mIsStatic; }
};
