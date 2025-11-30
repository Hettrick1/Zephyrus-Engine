#include "pch.h"
#include "HudManager.h"
#include <algorithm>

namespace Zephyrus::UI {
    void HudManager::AddElement(HudElement* pElement)
    {
        mHudElements.push_back(pElement);
    }

    void HudManager::Draw(Zephyrus::Render::RendererOpenGl& pRenderer)
    {
        std::sort(mHudElements.begin(), mHudElements.end(), [](const auto& a, const auto& b)
            {
                return a->GetDrawOrder() < b->GetDrawOrder();
            });

        for (auto& element : mHudElements) {
            element->Draw(pRenderer);
        }
    }

    void HudManager::Unload()
    {
        for (auto& element : mHudElements)
        {
            delete element;
            element = nullptr;
        }
        mHudElements.clear();
    }
}