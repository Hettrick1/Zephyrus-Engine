#include "HudContainer.h"

namespace Zephyrus::UI {
    HudContainer::HudContainer()
        : HudElement()
    {
    }

    void HudContainer::AddElement(HudElement* pElement)
    {
        mElements.push_back(pElement);
    }

    void HudContainer::Draw(Zephyrus::Render::RendererOpenGl& pRenderer)
    {
        for (auto& element : mElements) {
            element->Draw(pRenderer);
        }
    }
}
