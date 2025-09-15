#include "SceneHierarchyPanel.h"
#include "SceneManager.h"
#include "Scene.h"
#include "DebugRenderer.h"

int selected = 0;

SceneHierarchyPanel::SceneHierarchyPanel(const std::string& pName)
	: Panel(pName)
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

	auto actors = SceneManager::ActiveScene->GetAllActors();

	for (int i = 0; i < actors.size(); i++)
	{
		char label[32];

		sprintf(label, (actors[i]->GetName() + "_%i").c_str(), i);
		ImGui::PushID(label);
		if (ImGui::Selectable(actors[i]->GetName().c_str(), selected == i))
		{
			selected = i;
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
			SceneManager::ActiveScene->RemoveActor(mSelectedActor);
			delete mSelectedActor;
			actors = SceneManager::ActiveScene->GetAllActors();
			if (selected >= actors.size())
			{
				selected = actors.size() - 1;
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
	auto actors = SceneManager::ActiveScene->GetAllActors();
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

Actor* SceneHierarchyPanel::GetSelectedActor() const
{
	return mSelectedActor;
}
