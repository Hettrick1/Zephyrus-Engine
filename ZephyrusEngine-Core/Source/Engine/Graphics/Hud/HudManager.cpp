#include "HudManager.h"

void HudManager::AddElement(HudElement* element)
{
    mHudElements.push_back(element);
}

void HudManager::Draw(RendererOpenGl& renderer)
{
    for (auto& element : mHudElements) {
        element->Draw(renderer);
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
