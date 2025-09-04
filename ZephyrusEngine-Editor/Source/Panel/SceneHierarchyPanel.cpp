#include "SceneHierarchyPanel.h"
#include "SceneManager.h"
#include "Scene.h"

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
	static int selected = 0;

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
		ImGui::PopID();
	}
	ImGui::End();
	Panel::EndDraw();
}

Actor* SceneHierarchyPanel::GetSelectedActor() const
{
	return mSelectedActor;
}
