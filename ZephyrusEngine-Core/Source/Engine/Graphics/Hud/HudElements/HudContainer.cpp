#include "HudContainer.h"

HudContainer::HudContainer()
	: HudElement()
{
}

void HudContainer::AddElement(HudElement* element)
{
    mElements.push_back(element);
}

void HudContainer::Draw(RendererOpenGl& renderer)
{
    for (auto& element : mElements) {
        element->Draw(renderer);
    }
}
