#pragma once
#include "InputActions.h"

// Represents a 2D input axis, typically used for handling input from keyboard or mouse for movement or direction.
class Axis2DAction : public InputActions
{
public:
    // Constructor for axis with specific key bindings.
    Axis2DAction(SDL_Keycode positiveX, SDL_Keycode negativeX, SDL_Keycode positiveY, SDL_Keycode negativeY, std::string name);

    // Constructor for axis with only a name.
    Axis2DAction(std::string name);

    ActionType GetType() const override;

    void Update() override;

    Vector2D GetAxis() const;

private:
    bool mUseMouse;
    SDL_Keycode mPositiveX, mNegativeX, mPositiveY, mNegativeY;
    float x, y;
    // Checks if a specific key is currently pressed.
    bool IsKeyPressed(SDL_Keycode key) const;
};
