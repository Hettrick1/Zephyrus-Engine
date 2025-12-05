#include "SceneHierarchyPanel.h"
#include "SceneManager.h"
#include "Scene.h"
#include "DebugRenderer.h"

int selected = 0;

SceneHierarchyPanel::SceneHierarchyPanel(ISceneContext* pSceneContext, const std::string& pName)
	: Panel(pSceneContext, pName)
{
}

SceneHierarchyPanel::~SceneHierarchyPanel()
{
}

void SceneHierarchyPanel::Draw()
{
	if (!mDrawPanel)
	{
		return;
	}

	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());

	auto actors = mContext->GetActiveScene()->GetAllActors();

	for (int i = 0; i < actors.size(); i++)
	{
		char label[32];

		sprintf_s(label, (actors[i]->GetName() + "_%i").c_str(), i);
		ImGui::PushID(label);
		if (ImGui::Selectable(actors[i]->GetName().c_str(), selected == i))
		{
			selected = i;
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			ImGui::SetDragDropPayload("ACTOR", actors[i]->GetUUID().c_str(), actors[i]->GetUUID().size());
			ImGui::Text(actors[i]->GetName().c_str());
			ImGui::EndDragDropSource();
		}

		mSelectedActor = actors[selected];
		mSelectedActor->SetSelected(true);
		mSelectedActor->GetScene().GetRenderer()->SetSelectedActor(mSelectedActor);
		if (selected != i)
		{
			actors[i]->SetSelected(false);
		}
		ImGui::PopID();
	}
	if (ImGui::IsWindowFocused())
	{
		if (ImGui::IsKeyPressed(ImGuiKey_Delete))
		{
			mSelectedActor->Destroy();
			mContext->GetActiveScene()->RemoveActor(mSelectedActor);
			delete mSelectedActor;
			mSelectedActor = nullptr;
			actors = mContext->GetActiveScene()->GetAllActors();
			if (actors.size() == 0)
			{
				mSelectedActor = nullptr;
				ImGui::End();
				Panel::EndDraw();
				return;
			}
			if (selected >= static_cast<int>(actors.size()))
			{
				selected = static_cast<int>(actors.size()) - 1;
			}
			mSelectedActor = actors[selected];
			mSelectedActor->SetSelected(true);
			mSelectedActor->GetScene().GetRenderer()->SetSelectedActor(mSelectedActor);
		}
	}
	ImGui::End();
	Panel::EndDraw();
}

void SceneHierarchyPanel::ResetSelectedActor()
{
	auto actors = mContext->GetActiveScene()->GetAllActors();
	if(actors.size() > 0)
	{
		mSelectedActor = actors[0];
		selected = 0;
	}
	else
	{
		mSelectedActor = nullptr;
	}
}

Zephyrus::ActorComponent::Actor* SceneHierarchyPanel::GetSelectedActor() const
{
	return mSelectedActor;
}
