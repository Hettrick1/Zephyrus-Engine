#pragma once
#include "HudElement.h"
#include "RendererOpenGl.h"

#include <vector>

/**
 * @brief Container for HUD elements. Allows grouping and drawing multiple HUD elements together.
 */
class HudContainer
	: public HudElement
{
public:
    HudContainer();
    // Adds a HUD element to the container
    void AddElement(HudElement* pElement);

    void Draw(RendererOpenGl& pRenderer) override;

private:
    std::vector<HudElement*> mElements;
};
