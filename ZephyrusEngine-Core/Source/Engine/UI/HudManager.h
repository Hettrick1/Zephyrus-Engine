#pragma once
#include "HudElement.h"
#include "RendererOpenGl.h"
#include <vector>

/**
 * @brief Manages a collection of HUD elements and handles their rendering and unloading.
 */
namespace Zephyrus::UI {
    class HudManager
    {
    private:
        std::vector<HudElement*> mHudElements;
    public:
        // Adds a HUD element to the manager.
        void AddElement(HudElement* pElement);

        void Draw(Zephyrus::Render::RendererOpenGl& pRenderer);

        // Unloads all HUD elements managed by this manager.
        void Unload();
    };
}