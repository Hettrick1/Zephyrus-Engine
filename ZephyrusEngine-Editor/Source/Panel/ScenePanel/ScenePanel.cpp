#include "ScenePanel.h"

#include "Actor.h"
#include "Matrix4DRow.h"
#include "../../EditorApplication/EventSystem/EventSystem.h"
#include "../../EditorApplication/EventSystem/Event/SpawnPrefabEvent.h"
#include "Panel/SelectedActorAccessor.h"

ScenePanel::ScenePanel(ISceneContext* pSceneContext, const std::string& pName, unsigned int  pSceneRenderTexture, SelectedActorAccessor* actorAccessor)
	: Panel(pSceneContext, pName), mSceneRenderTexture(pSceneRenderTexture), mSelectedActorAccessor(actorAccessor)
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

	mSettingsBar.DrawGuizmoSettingsBar();

	ImVec2 size = ImGui::GetContentRegionAvail();
	mDimensions = Vector2D(size.x, size.y);

	ImGuizmo::SetDrawlist();
	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
	
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
			SpawnPrefabEvent* event = new SpawnPrefabEvent(mContext, prefabSource);
			EventSystem::DoEvent(event);
		}
		ImGui::EndDragDropTarget();
	}
	
	if (auto selectedActor = mSelectedActorAccessor->GetSelectedActor())
	{
		Matrix4DRow transform = selectedActor->GetTransformComponent().GetWorldTransform();
		float* matrix = transform.GetAsFloatPtr();
		ImGuizmo::Manipulate(cameraView.GetAsConstFloatPtr(), cameraProjection.GetAsConstFloatPtr(), ImGuizmo::TRANSLATE, ImGuizmo::WORLD, matrix, NULL, NULL, NULL, NULL);
	}
	
	ImGui::End();
	Panel::EndDraw();
}

