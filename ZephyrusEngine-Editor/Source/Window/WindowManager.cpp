#include "WindowManager.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Zephyrus::Editor::Window
{
    void WindowManager::DrawWindows()
    {
        if (mWindows.empty())
        {
            mIsOpen = false;
            return;
        }
        mIsOpen = true;
                
        std::vector<std::string> toRemove;
        
        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
        
        ImGui::Begin("SuperWindow", &mIsOpen, ImGuiWindowFlags_NoTitleBar);
        
        ImGuiID dockspace_id = ImGui::GetID("Zephyrus Engine");
        ImGui::DockSpace(dockspace_id, ImVec2(0,0), ImGuiDockNodeFlags_None);
        
        for (auto& [id, w] : mWindows)
        {
            if (!w->IsOpen())
            {
                toRemove.push_back(id);
                continue;
            }

            ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_Always);
            w->Draw();
        }
        for (auto& id : toRemove)
        {
            mWindows.erase(id);
        }

        if (!mIsOpen)
        {
            mWindows.clear();
        }
        
        ImGui::End();
    }

    void WindowManager::Unload()
    {
        mWindows.clear();
    }
}
