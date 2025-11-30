#pragma once
#include "InputAction.h"

// This class represents input actions that have a boolean state (pressed or not pressed),
// such as keyboard keys or mouse buttons. It allows querying and updating the state.
namespace Zephyrus::Inputs {
    class InputActionBool : public InputAction {
    public:
        InputActionBool(const std::string& name)
            : InputAction(name, ActionType::Boolean) {}

        std::function<void()> OnStarted;
        std::function<void()> OnTriggered;
        std::function<void()> OnReleased;

        void TriggerStarted() const { if (OnStarted)   OnStarted(); }
        void TriggerTriggered() const { if (OnTriggered) OnTriggered(); }
        void TriggerReleased() const { if (OnReleased)  OnReleased(); }
    };
}
