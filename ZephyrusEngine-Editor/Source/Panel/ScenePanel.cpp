#include "ScenePanel.h"
#include "../EditorApplication/EventSystem/EventSystem.h"
#include "../EditorApplication/EventSystem/Event/SpawnPrefabEvent.h"

ScenePanel::ScenePanel(const std::string& pName, GLuint pSceneRenderTexture)
	: Panel(pName), mSceneRenderTexture(pSceneRenderTexture)
{
}

ScenePanel::~ScenePanel()
{
}

void ScenePanel::Draw()
{
	if (!mDrawPanel)
	{
		return;
	}

	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());

	ImVec2 size = ImGui::GetContentRegionAvail();
	mDimensions = Vector2D(size.x, size.y);

	ImGui::Image(
		(ImTextureID)(intptr_t)mSceneRenderTexture,
		size,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	mIsHover = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB"))
		{
			std::string prefabSource((const char*)payload->Data, payload->DataSize);
			SpawnPrefabEvent* event = new SpawnPrefabEvent(prefabSource);
			EventSystem::DoEvent(event);
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::End();
	Panel::EndDraw();
}

