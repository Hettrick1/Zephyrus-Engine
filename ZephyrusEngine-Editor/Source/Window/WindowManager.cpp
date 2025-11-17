#include "WindowManager.h"
#include "Interface/ITexture2D.h"
#include "Assets.h"
#include "../EditorUI/ImGuiUtils.h"
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

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings;
        
        ImGui::Begin("Zephyrus Engine", nullptr, flags);
        
        ImGuiID dockspace_id = ImGui::GetID("Windows_Docker");
        ImGui::DockSpace(dockspace_id, ImVec2(0,0), ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton);
        
        if (ImGui::BeginMenuBar())
        {
            float barWidth = ImGui::GetWindowWidth();
            
            float buttonSize = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            float totalButtonsWidth = buttonSize * 4;
            float rightOffset = barWidth - totalButtonsWidth - 10.0f;

            ImGui::Text("Zephyrus Engine");
            
            ImGui::SetCursorPosX(rightOffset);
            Zephyrus::Assets::ITexture2D* fullScreenTex = Assets::AssetsManager::LoadTexture("Sprites/Icons/FullScreen16.png", "Sprites/Icons/FullScreen16.png");
            ImTextureID fullScreenIcon = (ImTextureID)(intptr_t)fullScreenTex->GetHandle();
            Zephyrus::Assets::ITexture2D* exitFullScreenTex = Assets::AssetsManager::LoadTexture("Sprites/Icons/ExitFullScreen16.png", "Sprites/Icons/ExitFullScreen16.png");
            ImTextureID exitFullScreenIcon = (ImTextureID)(intptr_t)exitFullScreenTex->GetHandle();

            ImVec2 iconSize(16, 16);
            
            ImVec2 btnSize(buttonSize * 2, buttonSize);

            // TODO : add button
            if (ZP::UI::CustomImageButton("FullScreen", isMaximized ? exitFullScreenIcon : fullScreenIcon, btnSize, iconSize))
            {
                if (!isMaximized)
                {
                    savedPos = ImGui::GetWindowPos();
                    savedSize = ImGui::GetWindowSize();
                    ImGui::SetWindowPos(ImVec2(0, 0));
                    ImGui::SetWindowSize(io.DisplaySize);
                    isMaximized = true;
                }
                else
                {
                    ImGui::SetWindowPos(savedPos);
                    ImGui::SetWindowSize(savedSize);
                    isMaximized = false;
                }   
            }
            
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.0f, 0.0f, 1.0f));
            if (ImGui::Button("X", ImVec2(buttonSize * 2, buttonSize)))
            {
                mIsOpen = false;
                isMaximized = false;
            }
            ImGui::PopStyleColor();

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
