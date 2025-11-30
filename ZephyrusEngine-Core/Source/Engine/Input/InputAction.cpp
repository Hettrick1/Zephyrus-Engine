#include "pch.h"
#include "InputAction.h"
#include "IActionListener.h"
#include "Actor.h"

namespace Zephyrus::Inputs {
	InputAction::InputAction(const std::string& pName, ActionType type)
		: mName(pName), mType(type)
	{
	}
	

	void InputAction::SetName(const std::string& pName)
	{
		mName = pName;
	}

	bool InputAction::IsBoundToKey(int key) const {
		for (int k : mKeys)
			if (k == key)
			{
				return true;
			}
		return false;
	}

	bool InputAction::IsBoundToMouse(int button) const {
		for (int b : mMouseButtons)
			if (b == button)
			{
				return true;
			}
		return false;
	}

}
