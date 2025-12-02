#pragma once

#include "Component.h"
#include "IActionListener.h"

using Zephyrus::Inputs::IActionListener;
using Zephyrus::Inputs::InputAction;
using Zephyrus::Inputs::ActionType;

namespace Zephyrus::Inputs
{
	class BooleanActions;
	class InputManager;
	class Axis2DAction;
}

using Zephyrus::Inputs::InputManager;
using Zephyrus::Inputs::BooleanActions;
using Zephyrus::Inputs::Axis2DAction;

namespace Zephyrus::ActorComponent {
	class EditorControllerComponent : public Component, IActionListener
	{
	private:
		float mSpeed = 10.0;
		float mYaw = 0.0;
		float mPitch = 0.0;
		float mMouseSensitivity = 0.1f;
		bool mCanMove = false;
		bool mInCapture = false;

		InputManager* mEditorInputManager{ nullptr };
	public:
		EditorControllerComponent(Actor* pOwner, int pUpdateOrder = 100);

		void OnActionStarted(InputAction* pAction) override;
		void OnActionTriggered(InputAction* pAction) override;
		void OnActionEnded(InputAction* pAction) override;

		void OnStart() override;

		void Update() override;

		void SetMovementSpeed(float pSpeed);

		void SetIsInSceneCapture(bool pInCapture);

		bool GetInCapture() const { return mInCapture; }
		bool GetCanMove() const { return mCanMove; }
		
		void SetInputManager(InputManager* pInputManager);
		void Rotate(Vector2D delta);
	};
}
