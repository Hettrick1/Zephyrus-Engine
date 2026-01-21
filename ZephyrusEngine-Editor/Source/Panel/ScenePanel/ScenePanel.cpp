#include "ScenePanel.h"

#include "Actor.h"
#include "Matrix4DRow.h"
#include "../../EditorApplication/EventSystem/EventSystem.h"
#include "../../EditorApplication/EventSystem/Event/SpawnPrefabEvent.h"
#include "EditorApplication/EventSystem/Event/SetTransformEvent.h"
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

		auto guizmoMode = mSettingsBar.GetGuizmoMode();
		auto guizmoOperation = mSettingsBar.GetGuizmoOperation();
		
		Matrix4DRow transform = selectedActor->GetTransformComponent().GetWorldTransform();
		float* matrix = transform.GetAsFloatPtr();
		ImGuizmo::Manipulate(cameraView.GetAsConstFloatPtr(), cameraProjection.GetAsConstFloatPtr(), guizmoOperation, guizmoMode, matrix, NULL, NULL, NULL, NULL);

		if (!ImGuizmo::IsUsing() && !mDeactivatedAfterEdit)
		{
			oldTransform = transform;
		}
		if (ImGuizmo::IsUsing())
		{
			mDeactivatedAfterEdit = true;
			switch (guizmoOperation)
			{
			case ImGuizmo::TRANSLATE :
				selectedActor->SetPosition(transform.GetTranslation());
				break;
			case ImGuizmo::ROTATE :
				selectedActor->SetRotation(transform.GetRotation());
				break;
			case ImGuizmo::SCALE :
				selectedActor->SetSize(transform.GetScale());
				break;
			default :
			break;
			}
		}
		if (!ImGuizmo::IsUsing() && mDeactivatedAfterEdit && transform != oldTransform)
		{
			switch (guizmoOperation)
			{
			case ImGuizmo::TRANSLATE :
				{
					SetActorPositionEvent* pos = new SetActorPositionEvent(selectedActor, oldTransform.GetTranslation(), transform.GetTranslation());
					EventSystem::DoEvent(pos);
					break;
				}
			case ImGuizmo::ROTATE :
				{
					SetActorRotationEvent* rot = new SetActorRotationEvent(selectedActor, oldTransform.GetRotation(), transform.GetRotation());
					EventSystem::DoEvent(rot);
					break;
				}
			case ImGuizmo::SCALE :
				{
					SetActorSizeEvent* size = new SetActorSizeEvent(selectedActor, oldTransform.GetScale(), transform.GetScale());
					EventSystem::DoEvent(size);
					break;
				}
			default :
				break;
			}
			mDeactivatedAfterEdit = false;
		}

	}
	
	ImGui::End();
	Panel::EndDraw();
}

