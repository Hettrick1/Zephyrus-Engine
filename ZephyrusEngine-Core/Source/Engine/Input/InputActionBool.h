#pragma once
#include "InputAction.h"

// This class represents input actions that have a boolean state (pressed or not pressed),
// such as keyboard keys or mouse buttons. It allows querying and updating the state.
namespace Zephyrus::Inputs {
    class InputActionBool : public InputAction
    {
    private:
        std::vector<int> mKeys;
        std::vector<int> mMouseButtons;
    public:
        InputActionBool(const std::string& name)
            : InputAction(name, ActionType::Boolean) {}

        std::function<void()> OnStarted;
        std::function<void()> OnTriggered;
        std::function<void()> OnReleased;

        void BindKey(int key) { mKeys.push_back(key); }
        bool IsBoundToKey(int key) const override
        {
            for (int k : mKeys)
                if (k == key)
                {
                    return true;
                }
            return false;
        }

        void BindMouseButton(int button) { mMouseButtons.push_back(button); }
        bool IsBoundToMouse(int button) const override
        {
            for (int k : mMouseButtons)
                if (k == button)
                {
                    return true;
                }
            return false;
        }
        
        void TriggerStarted() const { if (OnStarted)   OnStarted(); }
        void TriggerTriggered() const { if (OnTriggered) OnTriggered(); }
        void TriggerReleased() const { if (OnReleased)  OnReleased(); }

        std::vector<int> GetKeys() const { return mKeys; }
        std::vector<int> GetMouseButtons() const { return mMouseButtons; }
    };
}
