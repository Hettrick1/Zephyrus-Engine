#include "ContentBrowserPanel.h"

ContentBrowserPanel::ContentBrowserPanel(const std::string& pName)
	: Panel(pName)
{
}

ContentBrowserPanel::~ContentBrowserPanel()
{
}

void ContentBrowserPanel::Draw()
{
	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());
	ImGui::Text("Content browser panel");
	ImGui::End();
	Panel::EndDraw();
}
