#pragma once

#include "IActionListener.h"
#include "Component/Component.h"
#include "HudText.h"
#include "HudImage.h"

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

		void Deserialize(const rapidjson::Value& pData) override;
		void Serialize(Serialization::Json::JsonWriter& pWriter) override;

		void OnStart() override;

		static Component* Create(Actor* pOwner) { return new DoomMenuPC(pOwner); }

	public:
		void OnActionStarted(InputActions* action) override;
		void OnActionTriggered(InputActions* action) override;
		void OnActionEnded(InputActions* action) override;
	public:
		void Update() override;
	};
}

