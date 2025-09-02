#include "ScenePanel.h"

ScenePanel::ScenePanel(const std::string& pName, GLuint pSceneRenderTexture)
	: Panel(pName), mSceneRenderTexture(pSceneRenderTexture)
{
}

ScenePanel::~ScenePanel()
{
}

void ScenePanel::Draw()
{
	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());

	ImVec2 size = ImGui::GetContentRegionAvail();


	ImGui::Image(
		(ImTextureID)(intptr_t)mSceneRenderTexture,
		size,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	mIsHover = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

	ImGui::End();
	Panel::EndDraw();
}

