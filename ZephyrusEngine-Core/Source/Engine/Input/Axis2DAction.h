#pragma once
#include "InputActions.h"

// Represents a 2D input axis, typically used for handling input from keyboard or mouse for movement or direction.
class Axis2DAction : public InputActions
{
private:
    bool mUseMouse;
    SDL_Keycode mPositiveX, mNegativeX, mPositiveY, mNegativeY;
    float mX, mY;
    // Checks if a specific key is currently pressed.
    bool IsKeyPressed(SDL_Keycode pKey) const;
public:
    // Constructor for axis with specific key bindings.
    Axis2DAction(SDL_Keycode pPositiveX, SDL_Keycode pNegativeX, SDL_Keycode pPositiveY, SDL_Keycode pNegativeY, const std::string& pName);

    // Constructor for axis with only a name.
    Axis2DAction(const std::string& pName);

    ActionType GetType() const override;

    void Update() override;

    Vector2D GetAxis() const;
};
