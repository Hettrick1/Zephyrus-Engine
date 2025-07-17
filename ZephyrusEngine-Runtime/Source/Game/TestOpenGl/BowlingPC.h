#pragma once

#include "ActorMovementComponent.h"
#include "IActionListener.h"

class BowlingPC : public ActorMovementComponent, IActionListener
{
public:
	BowlingPC(Actor* pOwner, int pUpdateOrder = 100);
	BowlingPC() = delete;
	BowlingPC(const BowlingPC&) = delete;
	BowlingPC& operator=(const BowlingPC&) = delete;
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
	float mShootStrengh, mXPosition;
	unsigned int mCameraIndex;
	class BowlingPlayer* mPlayer;
};