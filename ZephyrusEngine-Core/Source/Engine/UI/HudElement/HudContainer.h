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

private:
    std::vector<HudElement*> mElements;
public:
    HudContainer();
    // Adds a HUD element to the container
    void AddElement(HudElement* pElement);

    void Draw(Zephyrus::Render::RendererOpenGl& pRenderer) override;
};
