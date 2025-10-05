#include "Panel.h"

Panel::Panel(ISceneContext* pSceneContext, const std::string& pName)
	: mContext{ pSceneContext }, mName{ pName }
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
