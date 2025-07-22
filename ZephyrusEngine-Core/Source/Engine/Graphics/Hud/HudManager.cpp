#include "HudManager.h"

void HudManager::AddElement(HudElement* pElement)
{
    mHudElements.push_back(pElement);
}

void HudManager::Draw(RendererOpenGl& pRenderer)
{
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
