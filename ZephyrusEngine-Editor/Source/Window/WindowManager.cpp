#include "WindowManager.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Zephyrus::Editor::Window
{
    void WindowManager::DrawWindows()
    {
        if (!mIsOpen)
        {
            return;
        }
        if (mWindows.empty())
        {
            mIsOpen = false;
            return;
        }
        mIsOpen = true;
                
        std::vector<std::string> toRemove;
        
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 size = ImVec2(800, 500);
        ImGui::SetNextWindowSize(size, ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2((io.DisplaySize.x - size.x) / 2, (io.DisplaySize.y - size.y) / 2), ImGuiCond_Appearing);
        isMaximized = false;
        
        ImGui::Begin("Zephyrus Engine", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings);
        
        ImGuiID dockspace_id = ImGui::GetID("Windows_Docker");
        ImGui::DockSpace(dockspace_id, ImVec2(0,0), ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton);
        
        if (ImGui::BeginMenuBar())
        {
            float barWidth = ImGui::GetWindowWidth();
            
            float buttonSize = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            float totalButtonsWidth = buttonSize * 2;
            float rightOffset = barWidth - totalButtonsWidth - 10.0f;

            ImGui::Text("Zephyrus Engine");
            
            ImGui::SetCursorPosX(rightOffset);
            
            ImGui::Button(isMaximized ? "[]]" : "[]", ImVec2(buttonSize, buttonSize));
            if (ImGui::IsItemClicked())
            {
                if (!isMaximized)
                {
                    // Save current
                    savedPos = ImGui::GetWindowPos();
                    savedSize = ImGui::GetWindowSize();

                    // Maximize
                    ImGui::SetWindowPos(ImVec2(0, 0));
                    ImGui::SetWindowSize(io.DisplaySize);

                    isMaximized = true;
                }
                else
                {
                    // Restore
                    ImGui::SetWindowPos(savedPos);
                    ImGui::SetWindowSize(savedSize);

                    isMaximized = false;
                }
            }
            if (ImGui::Button("X", ImVec2(buttonSize, buttonSize)))
            {
                mIsOpen = false;
            }

            ImGui::EndMenuBar();
        }
        
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
