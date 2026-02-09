#pragma once

#include "Component.h"
#include "FlipbookComponent.h"
#include "HudText.h"
#include "HudImage.h"
#include "Temp/DoomComponents/Weapons.h"

namespace Zephyrus::Assets
{
    class ITexture2D;
}

using Zephyrus::UI::HudText;
using Zephyrus::UI::HudImage;
using Zephyrus::UI::TextAlignment;

namespace Zephyrus::ActorComponent
{
    class TheDescentPlayer : public Component
    {
    public:
        TheDescentPlayer(Actor* pOwner, int updateOder = 0);
        ~TheDescentPlayer() override;

        void Deserialize(Serialization::IDeserializer& pReader) override;
        void Serialize(Serialization::ISerializer& pWriter) override;

        static Component* Create(Actor* pOwner) { return new TheDescentPlayer(pOwner); }

        void OnStart() override;
        void Update() override;
        void OnEnd() override;
    };
}
