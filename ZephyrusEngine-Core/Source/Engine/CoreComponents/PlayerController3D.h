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
	void OnActionStarted(InputActions* pAction) override;
	void OnActionTriggered(InputActions* pAction) override;
	void OnActionEnded(InputActions* pAction) override;
public:
	void Update() override;
public:
	void EnableDirection(Vector3D pDirection);
	void DisableDirection(Vector3D pDirection);
	void SetMovementSpeed(float pSpeed);
private:
	//temp
	bool mCanGoRight = false, mCanGoLeft = false, mCanGoUp = false, 
		 mCanGoDown = false, mCanGoForward = false, mCanGoBackward = false;
	float mSpeed = 0.0, mYaw = 0.0, mPitch = 0.0;
};

