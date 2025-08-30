#include "SceneHierarchyPanel.h"

SceneHierarchyPanel::SceneHierarchyPanel(const std::string& pName)
	: Panel(pName)
{
}

SceneHierarchyPanel::~SceneHierarchyPanel()
{
}

void SceneHierarchyPanel::Draw()
{
	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());
	ImGui::Text("Scene hierarchy panel");
	ImGui::End();
	Panel::EndDraw();
}
