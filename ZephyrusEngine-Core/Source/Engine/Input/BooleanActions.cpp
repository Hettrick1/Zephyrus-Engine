#include "BooleanActions.h"
#include <iostream>

namespace Zephyrus::Inputs {
	BooleanActions::BooleanActions(SDL_Keycode pKey, const std::string& pName)
		: mKey(pKey), mMouseButton(0), mState(false), InputActions(pName)
	{
	}

	BooleanActions::BooleanActions(Uint8 pMouseButton, const std::string& pName)
		: mKey(0), mMouseButton(pMouseButton), mState(false), InputActions(pName)
	{
	}

	ActionType BooleanActions::GetType() const
	{
		return ActionType::Boolean;
	}

	void BooleanActions::Update()
	{
		bool newState = false;
		if (mKey != 0)
		{
			newState = IsKeyPressed(mKey);
		}
		else if (mMouseButton != 0)
		{
			newState = IsMouseButtonPressed(mMouseButton);
		}
		if (newState) {
			NotifyListenersTriggered();
		}
		if (newState != mState) {
			if (newState == true) {
				NotifyListenersStarted();
			}
			else {
				NotifyListenersEnded();
			}
			mState = newState;
		}
	}

	bool BooleanActions::GetState() const
	{
		return mState;
	}

	bool BooleanActions::IsKeyPressed(SDL_Keycode pKey) const
	{
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		return keyState[SDL_GetScancodeFromKey(pKey)] != 0;
	}

	bool BooleanActions::IsMouseButtonPressed(Uint8 pMouseButton) const
	{
		return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(pMouseButton)) != 0;
	}
}