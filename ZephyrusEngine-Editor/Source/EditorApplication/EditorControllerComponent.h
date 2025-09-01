#pragma once

#include "Component.h"
#include "IActionListener.h"

class EditorControllerComponent : public Component, IActionListener
{
private:
	float mSpeed = 10.0;
	float mYaw = 0.0;
	float mPitch = 0.0;
	float mMouseSensitivity = 0.1f;
	bool mCanMove = false;
	bool mInCapture = false;
public:
	EditorControllerComponent(Actor* pOwner, int pUpdateOrder = 100);

	void OnActionStarted(InputActions* pAction) override;
	void OnActionTriggered(InputActions* pAction) override;
	void OnActionEnded(InputActions* pAction) override;

	void Update() override;

	void SetMovementSpeed(float pSpeed);

	void SetIsInSceneCapture(bool pInCapture);
};

