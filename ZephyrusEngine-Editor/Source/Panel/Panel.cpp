#include "Panel.h"

#include <imgui_internal.h>

Panel::Panel(ISceneContext* pSceneContext, const std::string& pName)
	: mContext{ pSceneContext }, mName{ pName }
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
