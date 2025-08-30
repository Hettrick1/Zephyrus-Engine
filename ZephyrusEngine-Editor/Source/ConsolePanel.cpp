#include "ConsolePanel.h"

ConsolePanel::ConsolePanel(const std::string& pName)
	: Panel(pName)
{
}

ConsolePanel::~ConsolePanel()
{
}

void ConsolePanel::Draw()
{
	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());
	ImGui::Text("Console panel");
	ImGui::End();
	Panel::EndDraw();

}
