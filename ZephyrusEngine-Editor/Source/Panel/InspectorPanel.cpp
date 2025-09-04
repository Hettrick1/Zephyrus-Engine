#include "InspectorPanel.h"
#include "Actor.h"
#include "EditorUI/ImGuiUtils.h"

InspectorPanel::InspectorPanel(const std::string& pName)
	: Panel(pName)
{
}

InspectorPanel::~InspectorPanel()
{
	mHierarchy = nullptr;
}

void InspectorPanel::Draw()
{
	if (!mDrawPanel)
	{
		return;
	}

	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());
	Actor* actor = mHierarchy->GetSelectedActor();
	if (actor)
	{
		static float h = 200.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::BeginChild("child1", ImVec2(0, h), true);
		ImGui::PopStyleVar();
		DrawActorComponents(actor);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::EndChild();

		ImGui::InvisibleButton("hsplitter", ImVec2(-1, 8.0f));
		if (ImGui::IsItemActive())
		{
			h += ImGui::GetIO().MouseDelta.y;
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		}
		ImGui::BeginChild("child3", ImVec2(0, 0), true);
		ImGui::EndChild();
		ImGui::PopStyleVar();	
	}
	else
	{
		ImGui::Text("No selected actor !");
	}
	ImGui::End();
	Panel::EndDraw();
}

void InspectorPanel::DrawActorComponents(Actor* pActor)
{
	static int selected = 0;
		ImGui::PushFont(ZP::UI::gFonts.medium);
		ImGui::Text(pActor->GetName().c_str());
		ImGui::PopFont();
		ImGui::Separator();

		auto components = pActor->GetComponents();

		for (int i = 0; i < components.size(); i++)
		{
			char label[32];

			sprintf(label, (components[i]->GetName() + "_%i").c_str(), i);
			ImGui::PushID(label);
			auto pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(pos.x + 10, pos.y + 5));
			if (ImGui::Selectable(components[i]->GetName().c_str(), selected == i))
			{
				selected = i;
			}
			ImGui::PopID();
		}
}

void InspectorPanel::SetSceneHierarchy(SceneHierarchyPanel* pHierarchy)
{
	mHierarchy = pHierarchy;
}
