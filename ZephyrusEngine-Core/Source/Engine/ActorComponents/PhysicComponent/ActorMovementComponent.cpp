#include "ActorMovementComponent.h"
#include "Actor.h"
#include "Timer.h"

ActorMovementComponent::ActorMovementComponent(Actor* pOwner, int pUpdateOrder)
	: Component(pOwner, pUpdateOrder), mSpeed(0.0f)
{
}

void ActorMovementComponent::SetSpeed(Vector3D pSpeed)
{
	mSpeed = pSpeed;
}

void ActorMovementComponent::SetSpeedX(float pSpeedX)
{
	mSpeed.x = pSpeedX;
}

void ActorMovementComponent::SetSpeedY(float pSpeedY)
{
	mSpeed.y = pSpeedY;
}

void ActorMovementComponent::SetSpeedZ(float pSpeedZ)
{
	mSpeed.z = pSpeedZ;
}

void ActorMovementComponent::Update()
{
	if(!Maths::NearZero(mSpeed.Length()))
	{
		Vector3D newPosition = mOwner->GetTransformComponent().GetPosition()
			+ (mOwner->GetTransformComponent().Right() * mSpeed.x
			+ Vector3D::unitZ * mSpeed.y 
			+ mOwner->GetTransformComponent().Forward() * mSpeed.z ) * Timer::deltaTime;
		mOwner->SetPosition(newPosition);
	}
}
