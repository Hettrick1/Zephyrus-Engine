#pragma once

#include "IActionListener.h"
#include "Component.h"

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
	class DoomPC : public IActionListener, public Component
	{
	public:
		DoomPC(Actor* pOwner, int pUpdateOrder = 100);
		DoomPC() = delete;
		DoomPC(const DoomPC&) = delete;
		DoomPC& operator=(const DoomPC&) = delete;
		~DoomPC();

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new DoomPC(pOwner); }
	public:
		void OnActionStarted(InputAction* action) override;
		void OnActionTriggered(InputAction* action) override;
		void OnActionEnded(InputAction* action) override;
	public:
		void Update() override;
	private:
		bool goRight, goLeft, goForward, goBackward;
	};
}

