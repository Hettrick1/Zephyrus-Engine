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
        }

        bool IsBoundToKey(int key) const override
        {
            if (keyValues.contains(key))
            {
                return true;
            }
            return false;
        }
        
        bool IsBoundToMouse(int button) const override
        {
            if (mouseValues.contains(button))
            {
                return true;
            }
            return false;
        }

        void TriggerStarted() const { if (OnStarted)   OnStarted(); }
        void TriggerTriggered(const Vector2D& v) {
            value = v;
            if (OnTriggered) OnTriggered(value);
        }
        void TriggerReleased() const { if (OnReleased)  OnReleased(); }

        void Reset() { value = {0,0}; }

        std::unordered_map<int, Vector2D> GetKeyValues() const {return keyValues;}
        std::unordered_map<int, Vector2D> GetMouseValues() const {return mouseValues;}
    };

}