#include "MaterialWindow.h"
#include "Assets.h"
#include <imgui.h> 

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
        ImGui::Text("Editing material: %s", mFilePath.c_str());

        for (auto prop : mMaterial->GetProperties())
        {
            mComponentPropertyDrawer->DrawProperty(prop, nullptr);
        }
        
        // TODO: ton UI material ici
        ImGui::End();
    }
}
