#pragma once
#include "InputActions.h"

// This class represents input actions that have a boolean state (pressed or not pressed),
// such as keyboard keys or mouse buttons. It allows querying and updating the state.
namespace Zephyrus::Inputs {
    class BooleanActions : public InputActions
    {
    private:
        bool mState;
        SDL_Keycode mKey;
        Uint8 mMouseButton;

        // Checks if the specified key is currently pressed
        bool IsKeyPressed(SDL_Keycode pKey) const;
        // Checks if the specified mouse button is currently pressed
        bool IsMouseButtonPressed(Uint8 pMouseButton) const;
    public:
        BooleanActions(SDL_Keycode pKey, const std::string& pName); // Constructor for keyboard key action
        BooleanActions(Uint8 pMouseButton, const std::string& pName); // Constructor for mouse button action

        ActionType GetType() const override;

        void Update() override; // Updates the current state of the action

        bool GetState() const; // Returns the current state (pressed or not)
    };
}
