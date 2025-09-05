#include "MenuPanel.h"
#include "EditorApplication/EditorApplication.h"
#include "EditorApplication/EventSystem/EventSystem.h"

MenuPanel::MenuPanel(const std::string& pName, EditorApplication* pApplication)
	: Panel(pName), mApplication(pApplication)
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
            bool canUndo = EventSystem::GetCanUndo();
            if (ImGui::MenuItem("Undo", nullptr, false, canUndo))
            {
                EventSystem::UndoLastEvent();
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                if (!canUndo) {
                    ImGui::SetTooltip("Nothing to undo");
                }
                else
                {
                    ImGui::SetTooltip(EventSystem::GetLastEventName().c_str());
                }
            }
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
            auto consolePanel = mApplication->GetPanelWithName(mApplication->consolePanelName);
            auto scenePanel = mApplication->GetPanelWithName(mApplication->scenePanelName);
            auto hierarchyPanel = mApplication->GetPanelWithName(mApplication->sceneHierarchyName);
            auto contentPanel = mApplication->GetPanelWithName(mApplication->contentBrowserName);
            auto inspectorPanel = mApplication->GetPanelWithName(mApplication->inspectorPanelName);
            auto prefabPanel = mApplication->GetPanelWithName(mApplication->prefabPanelName);
            if (ImGui::MenuItem("Console", nullptr, consolePanel->GetDrawPanel()))
            {
                consolePanel->SetDrawPanel(!consolePanel->GetDrawPanel());
            }
            if (ImGui::MenuItem("Scene", nullptr, scenePanel->GetDrawPanel()))
            {
                scenePanel->SetDrawPanel(!scenePanel->GetDrawPanel());
            }
            if (ImGui::MenuItem("Hierarchy", nullptr, hierarchyPanel->GetDrawPanel()))
            {
                hierarchyPanel->SetDrawPanel(!hierarchyPanel->GetDrawPanel());
            }
            if (ImGui::MenuItem("Content", nullptr, contentPanel->GetDrawPanel()))
            {
                contentPanel->SetDrawPanel(!contentPanel->GetDrawPanel());
            }
            if (ImGui::MenuItem("Inspector", nullptr, inspectorPanel->GetDrawPanel()))
            {
                inspectorPanel->SetDrawPanel(!inspectorPanel->GetDrawPanel());
            }
            if (ImGui::MenuItem("Prefabs", nullptr, prefabPanel->GetDrawPanel()))
            {
                prefabPanel->SetDrawPanel(!prefabPanel->GetDrawPanel());
            }
            ImGui::EndMenu();
        }
        ImGui::Separator();

        ImVec2 menuBarPos = ImGui::GetWindowPos();
        ImVec2 menuBarSize = ImGui::GetWindowSize();

        ImVec2 textSize = ImGui::CalcTextSize("ZephyrusEngine - v.0.0.1");
        ImVec2 versionTextPos = ImVec2(menuBarPos.x + menuBarSize.x - textSize.x - 15, menuBarPos.y);

        ImGui::SetCursorScreenPos(versionTextPos);
        ImGui::Text("ZephyrusEngine - v.0.0.1");

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
