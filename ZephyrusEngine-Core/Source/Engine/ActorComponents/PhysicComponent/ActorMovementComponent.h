#pragma once

#include "Component.h"
#include "Vector3D.h"
#include "Maths.h"

class Actor;

/**
 * @brief Component that manages the movement speed of an Actor.
 * Allows setting and getting the speed vector, and updates the Actor's position based on speed.
 */
class ActorMovementComponent : public Component
{
public:
	ActorMovementComponent(Actor* pOwner, int pUpdateOrder = 100);
	ActorMovementComponent() = delete;
	ActorMovementComponent(const ActorMovementComponent&) = delete;
	ActorMovementComponent& operator=(const ActorMovementComponent&) = delete;

	Vector3D GetSpeed() const { return mSpeed; }

	// Sets the speed vector
	void SetSpeed(Vector3D pSpeed);
	// Sets the X component of the speed
	void SetSpeedX(float pSpeedX);
	// Sets the Y component of the speed
	void SetSpeedY(float pSpeedY);
	// Sets the Z component of the speed
	void SetSpeedZ(float pSpeedZ);

	// Updates the Actor's position based on the current speed
	void Update() override;
protected:
	Vector3D mSpeed;
};
