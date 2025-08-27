#pragma once

#include "ActorMovementComponent.h"
#include "IActionListener.h"

class PlayerController2D : public ActorMovementComponent, IActionListener
{
private:
	bool mCanGoRight, mCanGoLeft, mCanGoUp, mCanGoDown;
public:
	PlayerController2D(Actor* pOwner, int pUpdateOrder = 100);
	PlayerController2D() = delete;
	PlayerController2D(const PlayerController2D&) = delete;
	PlayerController2D& operator=(const PlayerController2D&) = delete;

	void OnActionStarted(InputActions* pAction) override;
	void OnActionTriggered(InputActions* pAction) override;
	void OnActionEnded(InputActions* pAction) override;

	void Update() override;

	void EnableDirection(const Vector2D& pDirection);
	void DisableDirection(const Vector2D& pDirection);

};

