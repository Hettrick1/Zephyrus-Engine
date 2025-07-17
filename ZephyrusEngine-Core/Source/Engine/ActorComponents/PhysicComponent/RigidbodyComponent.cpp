#include "RigidbodyComponent.h"
#include "Physics/PhysicManager.h"
#include "ColliderComponent.h"
#include "Actor.h"
#include "Timer.h"
#include "Log.h"

RigidbodyComponent::RigidbodyComponent(Actor* pOwner, int pUpdateOrder)
	: Component(pOwner, pUpdateOrder), 
	mVelocity(0.0f), mAcceleration(0.0f), mMass(1.0f), mFriction(0.08f), mUseGravity(false), mGravity(-9.81f), 
	mBounciness(0.5f), mIsGrounded(false), mIsStatic(false)
{
    PhysicManager::Instance().RegisterRigidBody(pOwner, this);
    pOwner->SetRigidBody(this);
}

void RigidbodyComponent::Update()
{
	if (mUseGravity && mGravity != 0 && !mIsGrounded)
	{
		mAcceleration.z += mMass * mGravity * 0.01f;
	}

	mVelocity += mAcceleration;

	mVelocity *= (1.0f - mFriction * 0.1f);

	mOwner->GetTransformComponent().Translate(mVelocity * Timer::deltaTime);

	mAcceleration = 0;
}

void RigidbodyComponent::ApplyForce(Vector3D pForce)
{
	if (mUseGravity)
	{
		mAcceleration += pForce / mMass;
	}
}

void RigidbodyComponent::AddImpulse(Vector3D pImpulse)
{
    if (mMass > 0.0f)
    {
        mVelocity += pImpulse / mMass;
    }
}

void RigidbodyComponent::ResolveCollision(Vector3D pResolveForce)
{
    if (mIsStatic || mMass > 10000)
    {
        return;
    }

	mAcceleration = pResolveForce;
}

void RigidbodyComponent::OnCollisionEnter(ColliderComponent* otherCollider)
{
    
}

void RigidbodyComponent::OnCollisionStay(ColliderComponent* otherCollider)
{

}

void RigidbodyComponent::OnCollisionExit(ColliderComponent* otherCollider)
{
   
}

void RigidbodyComponent::SetVelocity(Vector3D pVelocity)
{
	mVelocity = pVelocity;
}

void RigidbodyComponent::SetMass(float pMass)
{
	mMass = (pMass > 0.0f) ? pMass : 1.0f;
}

void RigidbodyComponent::SetFriction(float pFriction)
{
	mFriction = pFriction;
}

void RigidbodyComponent::SetUseGravity(bool pUseGravity)
{
	mUseGravity = pUseGravity;
}

void RigidbodyComponent::SetIsGrounded(bool pIsGrounded)
{
    mIsGrounded = pIsGrounded;
}

void RigidbodyComponent::SetGravity(float pGravity)
{
    mGravity = pGravity;
}

void RigidbodyComponent::SetIsStatic(bool isStatic)
{
    mIsStatic = isStatic;
}
