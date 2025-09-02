#include "UtilsPanel.h"

UtilsPanel::UtilsPanel(const std::string& pName)
	: Panel(pName)
{
}

UtilsPanel::~UtilsPanel()
{
}

void UtilsPanel::Draw()
{
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0));
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, 45));

    ImGui::Begin("CenterPanel", nullptr, flags);

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImVec2 widgetSize = ImVec2(200, 30);

    ImVec2 pos = ImVec2((windowSize.x - widgetSize.x) * 0.5f, (windowSize.y - widgetSize.y) * 0.5f + (15 * 0.5f));
    ImGui::SetCursorPos(pos);
    ImGui::Button("Play", widgetSize);

    ImVec2 menuBarPos = ImGui::GetWindowPos();
    ImVec2 menuBarSize = ImGui::GetWindowSize();

    ImDrawList* draw_list = ImGui::GetForegroundDrawList();
    draw_list->AddLine(
        ImVec2(menuBarPos.x, menuBarPos.y + menuBarSize.y),
        ImVec2(menuBarPos.x + menuBarSize.x, menuBarPos.y + menuBarSize.y),
        IM_COL32(120, 120, 120, 255),
        0.8f
    );

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);
}
