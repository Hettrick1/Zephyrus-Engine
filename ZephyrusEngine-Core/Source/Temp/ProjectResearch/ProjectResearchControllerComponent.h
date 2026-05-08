#pragma once

#include "Component.h"
#include "IActionListener.h"
#include "HudImage.h"

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
    class ProjectResearchControllerComponent : public Component
    {
    private:
        Zephyrus::UI::HudImage* mCrossHair = nullptr;

        float mSpeed = 10.0f;
        float mYaw = 0.0f;
        float mPitch = 0.0f;

        float mMouseSensitivity = 0.1f;

        InputManager* mInputManager{ nullptr };
    public:
        ProjectResearchControllerComponent(Actor* pOwner, int pUpdateOrder = 100);

        ProjectResearchControllerComponent() = delete;
        ProjectResearchControllerComponent(const ProjectResearchControllerComponent&) = delete;
        ProjectResearchControllerComponent& operator=(const ProjectResearchControllerComponent&) = delete;
        ~ProjectResearchControllerComponent() override;

        void Deserialize(Serialization::IDeserializer& pReader) override;
        void Serialize(Serialization::ISerializer& pWriter) override;

        static Component* Create(Actor* pOwner) { return new ProjectResearchControllerComponent(pOwner); }

    public:
        void OnStart() override;

        void Update() override;

        void SetMovementSpeed(float pSpeed);

    private:
        void Rotate(Vector2D delta);
        void Move(Vector2D delta);
        void UpDown(float direction);

        void SelectNode();
    };
}


#pragma once
