#pragma once

#include "Component.h"
#include "IActionListener.h"

using Zephyrus::Inputs::IActionListener;
using Zephyrus::Inputs::InputAction;
using Zephyrus::Inputs::ActionType;

namespace Zephyrus::Inputs
{
    class BooleanActions;
    class InputManager;
    class Axis2DAction;
}

using Zephyrus::Inputs::InputManager;
using Zephyrus::Inputs::BooleanActions;
using Zephyrus::Inputs::Axis2DAction;

namespace Zephyrus::ActorComponent {
    class TheDescentPC : public Component
    {
    private:
        float mSpeed = 10.0;
        float mYaw = 0.0;
        float mPitch = 0.0;
        float mRoll = 0.0;
        
        float mMouseSensitivity = 0.1f;
        
        InputManager* mInputManager{ nullptr };
    public:
        TheDescentPC(Actor* pOwner, int pUpdateOrder = 100);
        
        TheDescentPC() = delete;
        TheDescentPC(const TheDescentPC&) = delete;
        TheDescentPC& operator=(const TheDescentPC&) = delete;
        ~TheDescentPC() override;
        
        void Deserialize(Serialization::IDeserializer& pReader) override;
        void Serialize(Serialization::ISerializer& pWriter) override;

        static Component* Create(Actor* pOwner) { return new TheDescentPC(pOwner); }

    public:        
        void OnStart() override;

        void Update() override;

        void SetMovementSpeed(float pSpeed);
        
        void Rotate(Vector2D delta);
        void Move(Vector2D delta);
        void UpDown(float direction);
        void Tilt(float direction);
    };
}


