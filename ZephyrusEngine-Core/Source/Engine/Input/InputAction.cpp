#include "pch.h"
#include "InputAction.h"
#include "IActionListener.h"
#include "Actor.h"

namespace Zephyrus::Inputs {
	InputAction::InputAction(const std::string& pName, ActionType type)
		: mName(pName), mType(type)
	{
	}
}
