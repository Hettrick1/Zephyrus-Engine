#include "MaterialWindow.h"

#include "AssetsManager.h"
#include "../../EditorUI/ImGuiUtils.h"
#include "Interface/ITexture2D.h"
#include <imgui.h> 

#include "EditorApplication/EventSystem/EventSystem.h"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include <filesystem>
#endif

namespace Zephyrus::Editor::Window
{
    MaterialWindow::MaterialWindow(const std::string& filePath, const std::string& title)
    : mFilePath(filePath), mTitle(title)
    {
    	mComponentPropertyDrawer = new ComponentPropertyDrawer();
        mMaterial = Assets::AssetsManager::LoadMaterial(filePath, filePath);
    }

    MaterialWindow::~MaterialWindow()
    {
        delete mComponentPropertyDrawer;
        mComponentPropertyDrawer = nullptr;
    }

    void MaterialWindow::Draw()
    {
        if (!mIsOpen) return;
        
        ImGui::Begin(GetTitle().c_str(), &mIsOpen, ImGuiWindowFlags_NoFocusOnAppearing);
        
        ImGui::PushFont(ZP::UI::gFonts.medium);
        Zephyrus::Assets::ITexture2D* undoTex = Assets::AssetsManager::LoadTexture("Sprites/Icons/undo20.png", "Sprites/Icons/undo20.png");
        ImTextureID undoIcon = (ImTextureID)(intptr_t)undoTex->GetHandle();
        Zephyrus::Assets::ITexture2D* redoTex = Assets::AssetsManager::LoadTexture("Sprites/Icons/redo20.png", "Sprites/Icons/redo20.png");
        ImTextureID redoIcon = (ImTextureID)(intptr_t)redoTex->GetHandle();
        Zephyrus::Assets::ITexture2D* saveTex = Assets::AssetsManager::LoadTexture("Sprites/Icons/save20.png", "Sprites/Icons/save20.png");
        ImTextureID saveIcon = (ImTextureID)(intptr_t)saveTex->GetHandle();
        Zephyrus::Assets::ITexture2D* openTex = Assets::AssetsManager::LoadTexture("Sprites/Icons/open20.png", "Sprites/Icons/open20.png");
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
            std::filesystem::path path = mFilePath;
            ShellExecuteA(nullptr, "open", path.make_preferred().string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        }
#endif
        ImGui::PopFont();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        auto properties = mMaterial->GetProperties();
        for (unsigned int i = 0; i < properties.size(); i++)
        {
            mComponentPropertyDrawer->DrawProperty(std::to_string(i), properties[i], nullptr);
            ImGui::Dummy(ImVec2(0, 10));
        }
        
        // TODO: ton UI material ici
        ImGui::End();
    }
}
