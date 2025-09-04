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
	static int selected = -1;

	auto actors = SceneManager::ActiveScene->GetAllActors();

	for (int i = 0; i < actors.size(); i++)
	{
		char label[32];

		sprintf(label, (actors[i]->GetName() + "_%i").c_str(), i);
		if (ImGui::Selectable(label, selected == i))
		{
			selected = i;
		}
	}
	ImGui::End();
	Panel::EndDraw();
}
