#pragma once

#include "ActorMovementComponent.h"
#include "IActionListener.h"

class PlayerController3D : public ActorMovementComponent, IActionListener
{
public:
	PlayerController3D(Actor* pOwner, int pUpdateOrder = 100);
	PlayerController3D() = delete;
	PlayerController3D(const PlayerController3D&) = delete;
	PlayerController3D& operator=(const PlayerController3D&) = delete;
public:
	void OnActionStarted(InputActions* action) override;
	void OnActionTriggered(InputActions* action) override;
	void OnActionEnded(InputActions* action) override;
public:
	void Update() override;
public:
	void EnableDirection(Vector3D pDirection);
	void DisableDirection(Vector3D pDirection);
	void SetMovementSpeed(float pSpeed);
private:
	bool mCanGoRight, mCanGoLeft, mCanGoUp, mCanGoDown, mCanGoForward, mCanGoBackward;
	float mSpeed, mYaw, mPitch;
};

