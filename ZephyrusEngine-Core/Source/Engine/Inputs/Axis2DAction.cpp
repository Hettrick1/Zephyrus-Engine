#include "Axis2DAction.h"

Axis2DAction::Axis2DAction(SDL_Keycode positiveX, SDL_Keycode negativeX, SDL_Keycode positiveY, SDL_Keycode negativeY, std::string name)
    : mPositiveX(positiveX), mNegativeX(negativeX), mPositiveY(positiveY), mNegativeY(negativeY),
      x(0), y(0), InputActions(name), mUseMouse(false)
{
}

Axis2DAction::Axis2DAction(std::string name)
    : mPositiveX(SDLK_UNKNOWN), mNegativeX(SDLK_UNKNOWN), mPositiveY(SDLK_UNKNOWN), mNegativeY(SDLK_UNKNOWN),
      x(0), y(0), mUseMouse(true), InputActions(name)
{
}

ActionType Axis2DAction::GetType() const
{
    return ActionType::Axis2D;
}

void Axis2DAction::Update()
{
    float newX = 0, newY = 0;

    if (mUseMouse) { // mouse mode
        int deltaX = 0, deltaY = 0;
        SDL_GetRelativeMouseState(&deltaX, &deltaY);
        newX = static_cast<float>(deltaX);
        newY = static_cast<float>(deltaY);
    }
    else { // keyboard mode
        newX = static_cast<float>((IsKeyPressed(mPositiveX) ? 1 : 0) - (IsKeyPressed(mNegativeX) ? 1 : 0));
        newY = static_cast<float>((IsKeyPressed(mPositiveY) ? 1 : 0) - (IsKeyPressed(mNegativeY) ? 1 : 0));
    }

    bool hasChanged = (newX != x || newY != y);
    bool isActive = (newX != 0 || newY != 0);

    if (hasChanged) {
        x = newX;
        y = newY;

        if (isActive) {
            NotifyListenersStarted();
            NotifyListenersTriggered(); 
        }
        else {
            NotifyListenersEnded();
        }
    }

    if (isActive) {
        x = newX;
        y = newY;
        NotifyListenersTriggered();
    }
}

Vector2D Axis2DAction::GetAxis() const
{
    return {x, y};
}

bool Axis2DAction::IsKeyPressed(SDL_Keycode key) const
{
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    return keyState[SDL_GetScancodeFromKey(key)] != 0;
}
