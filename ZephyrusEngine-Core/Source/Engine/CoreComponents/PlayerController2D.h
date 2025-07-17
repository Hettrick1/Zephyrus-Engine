#pragma once

#include "ActorMovementComponent.h"
#include "IActionListener.h"

class PlayerController2D : public ActorMovementComponent, IActionListener
{
public:
	PlayerController2D(Actor* pOwner, int pUpdateOrder = 100);
	PlayerController2D() = delete;
	PlayerController2D(const PlayerController2D&) = delete;
	PlayerController2D& operator=(const PlayerController2D&) = delete;
public:
	void OnActionStarted(InputActions* action) override;
	void OnActionTriggered(InputActions* action) override;
	void OnActionEnded(InputActions* action) override;
public:
	void Update() override;
public:
	void EnableDirection(Vector2D pDirection);
	void DisableDirection(Vector2D pDirection);
private:
	bool mCanGoRight, mCanGoLeft, mCanGoUp, mCanGoDown;
};

