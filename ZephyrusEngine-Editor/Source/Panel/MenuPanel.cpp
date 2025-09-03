#include "MenuPanel.h"

MenuPanel::MenuPanel(const std::string& pName)
	: Panel(pName)
{
}

MenuPanel::~MenuPanel()
{
}

void MenuPanel::Draw()
{
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0));
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // examples for now
            if (ImGui::MenuItem("New")) { /* action */ }
            if (ImGui::MenuItem("Open")) { /* action */ }
            if (ImGui::MenuItem("Exit")) { /* action */ }
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo")) {/* action */ }
            if (ImGui::MenuItem("Redo")) {/* action */ }
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::MenuItem("Preferences")) {/* action */ }
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Panels"))
        {
            if (ImGui::MenuItem("Console", nullptr, true)) 
            {/* action */ }
            if (ImGui::MenuItem("Scene", nullptr, true))
            {/* action */ }
            if (ImGui::MenuItem("Hierarchy", nullptr, true))
            {/* action */ }
            if (ImGui::MenuItem("Content", nullptr, true))
            {/* action */ }
            if (ImGui::MenuItem("Inspector", nullptr, true))
            {/* action */
            }
            ImGui::EndMenu();
        }
        ImGui::Separator();

        ImVec2 menuBarPos = ImGui::GetWindowPos();
        ImVec2 menuBarSize = ImGui::GetWindowSize();
        ImGui::EndMainMenuBar();

        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddLine(
            ImVec2(menuBarPos.x, menuBarPos.y + menuBarSize.y),
            ImVec2(menuBarPos.x + menuBarSize.x, menuBarPos.y + menuBarSize.y),
            IM_COL32(120, 120, 120, 255),
            0.8f
        );
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(4);

}
