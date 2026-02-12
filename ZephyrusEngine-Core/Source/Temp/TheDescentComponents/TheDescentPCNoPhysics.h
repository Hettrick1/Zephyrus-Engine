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
    class TheDescentPCNoPhysics : public Component
    {
    private:
        float mSpeed = 0.001f;
        float mYaw = 0.0f;
        float mPitch = 0.0f;
        float mRoll = 0.0f;

        float mTiltSpeed = 25.0f;
        
        float mMouseSensitivity = 0.1f;
        
        InputManager* mInputManager{ nullptr };
    public:
        TheDescentPCNoPhysics(Actor* pOwner, int pUpdateOrder = 100);
        
        TheDescentPCNoPhysics() = delete;
        TheDescentPCNoPhysics(const TheDescentPCNoPhysics&) = delete;
        TheDescentPCNoPhysics& operator=(const TheDescentPCNoPhysics&) = delete;
        ~TheDescentPCNoPhysics() override;
        
        void Deserialize(Serialization::IDeserializer& pReader) override;
        void Serialize(Serialization::ISerializer& pWriter) override;

        static Component* Create(Actor* pOwner) { return new TheDescentPCNoPhysics(pOwner); }

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


