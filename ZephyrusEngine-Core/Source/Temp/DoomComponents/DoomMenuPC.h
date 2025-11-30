#pragma once

#include "IActionListener.h"
#include "Component/Component.h"
#include "HudText.h"
#include "HudImage.h"

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
	class DoomMenuPC : public IActionListener, public Component
	{
	private:
		Zephyrus::UI::HudImage* mDoomMenu = nullptr;
		Zephyrus::UI::HudText* mPressEnter = nullptr;
		float mTimer = 0;
	public:
		DoomMenuPC(Actor* pOwner, int pUpdateOrder = 100);
		DoomMenuPC() = delete;
		DoomMenuPC(const DoomMenuPC&) = delete;
		DoomMenuPC& operator=(const DoomMenuPC&) = delete;
		~DoomMenuPC();

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		void OnStart() override;

		static Component* Create(Actor* pOwner) { return new DoomMenuPC(pOwner); }

	public:
		void OnActionStarted(InputAction* action) override;
		void OnActionTriggered(InputAction* action) override;
		void OnActionEnded(InputAction* action) override;
	public:
		void Update() override;
	};
}

