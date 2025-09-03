#include "InspectorPanel.h"

InspectorPanel::InspectorPanel(const std::string& pName)
	: Panel(pName)
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::Draw()
{
	if (!mDrawPanel)
	{
		return;
	}

	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());
	ImGui::Text("Inspector panel");
	ImGui::End();
	Panel::EndDraw();
}
