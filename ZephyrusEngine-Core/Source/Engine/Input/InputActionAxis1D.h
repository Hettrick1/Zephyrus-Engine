#pragma once
#include "InputAction.h"

namespace Zephyrus::Inputs {

    class InputActionAxis1D : public InputAction {
    public:
        InputActionAxis1D(const std::string& name)
            : InputAction(name, ActionType::Axis1D) {}
        
        std::unordered_map<int,float> keyValues;
        std::unordered_map<int,float> mouseValues;

        float value = 0.f;
        bool IsDown = false;
        
        std::function<void()> OnStarted;
        std::function<void(float)> OnTriggered;
        std::function<void()> OnReleased;

        void BindKeyValue(int key, float v)
        {
            keyValues[key] = v;
        }

        void BindMouseValue(int key, float v)
        {
            mouseValues[key] = v;
        }

        void TriggerStarted() const { if (OnStarted)   OnStarted(); }
        void TriggerTriggered(float delta)
        {
            value += delta;
            if (OnTriggered) OnTriggered(value);
        }
        void TriggerReleased() const { if (OnReleased)  OnReleased(); }
        
        void Reset() { value = 0; }
    };

}