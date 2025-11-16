#include "MaterialWindow.h"
#include <imgui.h> 

namespace Zephyrus::Editor::Window
{
    MaterialWindow::MaterialWindow(const std::string& filePath, const std::string& title)
    : mFilePath(filePath), mTitle(title)
    {
    }

    void MaterialWindow::Draw()
    {
        if (!mIsOpen) return;
        
        ImGui::Begin(GetTitle().c_str(), &mIsOpen, ImGuiWindowFlags_NoFocusOnAppearing);
        ImGui::Text("Editing material: %s", mFilePath.c_str());
        // TODO: ton UI material ici
        ImGui::End();
    }
}
