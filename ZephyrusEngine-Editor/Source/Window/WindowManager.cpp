#include "WindowManager.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Zephyrus::Editor::Window
{
    void WindowManager::DrawWindows()
    {
        if (mWindows.empty())
        {
            return;
        }
        
        std::vector<std::string> toRemove;
        for (auto& [id, w] : mWindows)
        {
            if (!w->IsOpen()) { toRemove.push_back(id); continue; }
            w->Draw();
        }

        for (auto& id : toRemove)
        {
            mWindows.erase(id);
        }
        
        if (mWindows.empty())
        {
        }
    }

    void WindowManager::Unload()
    {
        mWindows.clear();
    }
}
