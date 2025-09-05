#include "InspectorPanel.h"
#include "Actor.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/Event/RenameActorEvent.h"
#include "EditorApplication/EventSystem/EventSystem.h"

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

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3.0f, 8.0f));
		ImGui::BeginChild("child1", ImVec2(0, 150), true);

		static bool isActive = true;
		if (ImGui::Checkbox("Active", &isActive))
		{
			if (isActive)
				//TODO : Set active event with undo
				actor->SetActive(ActorState::Active);
			else
				actor->SetActive(ActorState::Paused);
		}

		ImGui::SameLine();

		char buffer[64];
		strncpy(buffer, actor->GetName().c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			RenameActorEvent* event = new RenameActorEvent(actor, std::string(buffer));
			EventSystem::DoEvent(event);
		}

		ImGui::Separator();

		float labelWidth = 70.0f;

		float position[3];
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Position");
		ImGui::SameLine(labelWidth);
		ImGui::InputFloat3("##Position", position, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

		float rotation[3];
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Rotation");
		ImGui::SameLine(labelWidth);
		ImGui::InputFloat3("##Rotation", rotation, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

		float size[3];
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Size");
		ImGui::SameLine(labelWidth);
		ImGui::InputFloat3("##Size", size, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::BeginChild("child2", ImVec2(0, h), true);
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
		ImGui::Text("Components");
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
