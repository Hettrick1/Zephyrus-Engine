#pragma once
#include "InputAction.h"

#include <unordered_map>
#include "Vector2D.h"

namespace Zephyrus::Inputs {

    class InputActionAxis2D : public InputAction {
    public:
        InputActionAxis2D(const std::string& name)
            : InputAction(name, ActionType::Axis2D) {}

        // key -> vector contribution
        std::unordered_map<int, Vector2D> keyValues;
        std::unordered_map<int, Vector2D> mouseValues;

        Vector2D value = {0,0};
        
        std::function<void()> OnStarted;
        std::function<void(Vector2D)> OnTriggered;
        std::function<void()> OnReleased;

        void BindKeyValue(int key, const Vector2D& v)
        {
            keyValues[key] = v;
        }
        void BindMouseValue(int key, const Vector2D& v)
        {
            mouseValues[key] = v;
            IsMouseAxis = true;
        }

        void TriggerStarted() const { if (OnStarted)   OnStarted(); }
        void TriggerTriggered(const Vector2D& v) {
            value += v;
            if (OnTriggered) OnTriggered(value);
        }
        void TriggerReleased() const { if (OnReleased)  OnReleased(); }

        void Reset() { value = {0,0}; }
    };

}