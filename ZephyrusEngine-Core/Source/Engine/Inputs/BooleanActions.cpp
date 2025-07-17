#include "BooleanActions.h"
#include <iostream>

BooleanActions::BooleanActions(SDL_Keycode key, std::string name)
	: mKey(key), mMouseButton(0), mState(false), InputActions(name)
{
}

BooleanActions::BooleanActions(Uint8 mouseButton, std::string name)
	: mKey(0), mMouseButton(mouseButton), mState(false), InputActions(name)
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

bool BooleanActions::IsKeyPressed(SDL_Keycode key) const
{
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);
	return keyState[SDL_GetScancodeFromKey(key)] != 0;
}

bool BooleanActions::IsMouseButtonPressed(Uint8 mouseButton) const
{
	return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(mouseButton)) != 0;
}