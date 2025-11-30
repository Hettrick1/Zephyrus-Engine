#include "pch.h"
#include "InputActionAxis2D.h"
namespace Zephyrus::Inputs {
    // Axis2DAction::Axis2DAction(SDL_Keycode pPositiveX, SDL_Keycode pNegativeX, SDL_Keycode pPositiveY, SDL_Keycode pNegativeY, const std::string& pName)
    //     : mPositiveX(pPositiveX), mNegativeX(pNegativeX), mPositiveY(pPositiveY), mNegativeY(pNegativeY),
    //     mX(0), mY(0), InputActions(pName), mUseMouse(false)
    // {
    // }
    //
    // Axis2DAction::Axis2DAction(const std::string& pName)
    //     : mPositiveX(SDLK_UNKNOWN), mNegativeX(SDLK_UNKNOWN), mPositiveY(SDLK_UNKNOWN), mNegativeY(SDLK_UNKNOWN),
    //     mX(0), mY(0), mUseMouse(true), InputActions(pName)
    // {
    // }
    //
    // ActionType Axis2DAction::GetType() const
    // {
    //     return ActionType::Axis2D;
    // }
    //
    // void Axis2DAction::Update()
    // {
    //     float newX = 0, newY = 0;
    //
    //     if (mUseMouse) { // mouse mode
    //         int deltaX = 0, deltaY = 0;
    //         SDL_GetRelativeMouseState(&deltaX, &deltaY);
    //         newX = static_cast<float>(deltaX);
    //         newY = static_cast<float>(deltaY);
    //     }
    //     else { // keyboard mode
    //         newX = static_cast<float>((IsKeyPressed(mPositiveX) ? 1 : 0) - (IsKeyPressed(mNegativeX) ? 1 : 0));
    //         newY = static_cast<float>((IsKeyPressed(mPositiveY) ? 1 : 0) - (IsKeyPressed(mNegativeY) ? 1 : 0));
    //     }
    //
    //     bool hasChanged = (newX != mX || newY != mY);
    //     bool isActive = (newX != 0 || newY != 0);
    //
    //     if (hasChanged) {
    //         mX = newX;
    //         mY = newY;
    //
    //         if (isActive) {
    //             NotifyListenersStarted();
    //             NotifyListenersTriggered();
    //         }
    //         else {
    //             NotifyListenersEnded();
    //         }
    //     }
    //
    //     if (isActive) {
    //         mX = newX;
    //         mY = newY;
    //         NotifyListenersTriggered();
    //     }
    // }
    //
    // Vector2D Axis2DAction::GetAxis() const
    // {
    //     return { mX, mY };
    // }
    //
    // bool Axis2DAction::IsKeyPressed(SDL_Keycode pKey) const
    // {
    //     const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    //     return keyState[SDL_GetScancodeFromKey(pKey)] != 0;
    // }
}