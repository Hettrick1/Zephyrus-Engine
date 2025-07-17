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
    void AddElement(HudElement* element);

    void Draw(RendererOpenGl& renderer) override;

private:
    std::vector<HudElement*> mElements;
};
