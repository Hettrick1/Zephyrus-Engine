#include "Panel.h"

Panel::Panel(const std::string& pName)
	: mName(pName)
{
}

Panel::~Panel()
{
}

void Panel::Update()
{

}

void Panel::BeginDraw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
}

void Panel::EndDraw()
{
	ImGui::PopStyleVar(2);
}
