#pragma once

#include "IActionListener.h"
#include "Component/Component.h"

using Zephyrus::Inputs::IActionListener;
using Zephyrus::Inputs::InputActions;
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
	class PlayerController2D : public Component, IActionListener
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
}

