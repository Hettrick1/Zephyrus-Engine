#pragma once
#include "HudElement.h"
#include "RendererOpenGl.h"
#include <vector>

/**
 * @brief Manages a collection of HUD elements and handles their rendering and unloading.
 */
class HudManager
{
public:
    // Adds a HUD element to the manager.
    void AddElement(HudElement* pElement);

    void Draw(RendererOpenGl& pRenderer);

    // Unloads all HUD elements managed by this manager.
    void Unload();
private:
    std::vector<HudElement*> mHudElements;
};
