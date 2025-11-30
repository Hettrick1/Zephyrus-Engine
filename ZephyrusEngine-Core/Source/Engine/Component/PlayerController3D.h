#pragma once

#include "IActionListener.h"
#include "Component/Component.h"

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


namespace Zephyrus::ActorComponent
{
	class PlayerController3D : public Component, IActionListener
	{
	private:
		//temp
		bool mCanGoRight = false, mCanGoLeft = false, mCanGoUp = false,
			mCanGoDown = false, mCanGoForward = false, mCanGoBackward = false;
		float mSpeed = 0.0, mYaw = 0.0, mPitch = 0.0;
	public:
		PlayerController3D(Actor* pOwner, int pUpdateOrder = 100);
		PlayerController3D() = delete;
		PlayerController3D(const PlayerController3D&) = delete;
		PlayerController3D& operator=(const PlayerController3D&) = delete;

		void OnActionStarted(InputAction* pAction) override;
		void OnActionTriggered(InputAction* pAction) override;
		void OnActionEnded(InputAction* pAction) override;

		void Update() override;

		void SetMovementSpeed(float pSpeed);
	};
}
