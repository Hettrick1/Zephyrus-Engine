#include "MaterialWindow.h"

#include "AssetsManager.h"
#include "../../EditorUI/ImGuiUtils.h"
#include "Interface/ITexture2D.h"
#include "EditorUI/EditorContentIds.h"
#include <imgui.h> 

#include "EditorApplication/EventSystem/EventSystem.h"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <filesystem>
#endif

namespace Zephyrus::Editor::Window
{
    MaterialWindow::MaterialWindow(const std::string& matId, const std::string& title)
    : mMaterialId(matId), mTitle(title)
    {
    	mComponentPropertyDrawer = new ComponentPropertyDrawer();
        mMaterial = Assets::AssetsManager::GetInstance().LoadMaterial(matId);
    }

    MaterialWindow::~MaterialWindow()
    {
        delete mComponentPropertyDrawer;
        mComponentPropertyDrawer = nullptr;
    }

    void MaterialWindow::Draw()
    {
        if (!mIsOpen) return;
        
        ImGui::Begin(GetTitle().c_str(), &mIsOpen);
        
        ImGui::PushFont(ZP::UI::gFonts.medium);
        Zephyrus::Assets::ITexture2D* undoTex = Assets::AssetsManager::GetInstance().LoadTexture(TEX_UNDO20_ICON);
        ImTextureID undoIcon = (ImTextureID)(intptr_t)undoTex->GetHandle();
        Zephyrus::Assets::ITexture2D* redoTex = Assets::AssetsManager::GetInstance().LoadTexture(TEX_REDO20_ICON);
        ImTextureID redoIcon = (ImTextureID)(intptr_t)redoTex->GetHandle();
        Zephyrus::Assets::ITexture2D* saveTex = Assets::AssetsManager::GetInstance().LoadTexture(TEX_SAVE20_ICON);
        ImTextureID saveIcon = (ImTextureID)(intptr_t)saveTex->GetHandle();
        Zephyrus::Assets::ITexture2D* openTex = Assets::AssetsManager::GetInstance().LoadTexture(TEX_OPEN20_ICON);
        ImTextureID openIcon = (ImTextureID)(intptr_t)openTex->GetHandle();

        ImVec2 iconSize(20, 20);
        float buttonSize = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y;
        ImVec2 btnSize(buttonSize * 2, buttonSize * 1.1f);
        
        if (ZP::UI::CustomImageButton("Undo", undoIcon, btnSize, iconSize))
        {
            EventSystem::UndoLastEvent();
        }
        
        ImGui::SameLine();
        
        if (ZP::UI::CustomImageButton("Redo", redoIcon, btnSize, iconSize))
        {
            EventSystem::RedoLastUndo();
        }
        
        ImGui::SameLine();
        
        if (ZP::UI::CustomImageButton("Save", saveIcon, btnSize, iconSize))
        {
            mMaterial->Save();
        }
        
#ifdef _WIN32
        ImGui::SameLine();
        if (ZP::UI::CustomImageButton("Open", openIcon, btnSize, iconSize))
        {
            std::filesystem::path path = Assets::AssetsManager::GetInstance().GetFileDatabase().GetPathFromID(mMaterialId);
            ShellExecuteA(nullptr, "open", path.make_preferred().string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        }
#endif
        ImGui::PopFont();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::BeginChild("Properties");
        auto properties = mMaterial->GetProperties();
        for (unsigned int i = 0; i < properties.size(); i++)
        {
            mComponentPropertyDrawer->DrawProperty(std::to_string(i), properties[i], nullptr);
            ImGui::Dummy(ImVec2(0, 10));
        }
        ImGui::EndChild();
        ImGui::End();
    }
}
