#include "pch.h"
#include "HudContainer.h"

namespace Zephyrus::UI {
    HudContainer::HudContainer(ISceneContext* pContext)
        : HudElement(pContext)
    {
    }

    void HudContainer::AddElement(HudElement* pElement)
    {
        mElements.push_back(pElement);
    }

    void HudContainer::Draw(Zephyrus::Render::IRenderer& pRenderer)
    {
        for (auto& element : mElements) {
            element->Draw(pRenderer);
        }
    }
}
