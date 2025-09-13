#include "InspectorPanel.h"
#include "Actor.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/Event/RenameActorEvent.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "EditorApplication/EventSystem/Event/SetTransformEvent.h"
#include "Assets.h"
#include "ComponentFactory.h"
#include "Log.h"

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

		DrawActorInfos(actor);
		
		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::BeginChild("child2", ImVec2(0, h), true);
		ImGui::PopStyleVar();

		DrawActorComponents(actor);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::EndChild();

		DrawSplitterButton(h);

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

		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + 15);

		if (ImGui::Button("+ Add Component"))
		{
			ImGui::OpenPopup("AddCompMenu");
		}

		if (ImGui::BeginPopup("AddCompMenu", ImGuiWindowFlags_NoMove))
		{
			for (auto componentType : ComponentFactory::Instance().GetComponentNames())
			{
				// There are some components that cannot be added because they are temporary (doom enemy component needs to be in a prefab to work)
				if (componentType == "SkySphereComponent" || componentType == "DoomEnemyComponent") 
				{
					continue;
				}
				if (ImGui::Button(componentType.c_str()))
				{
					Component* c = ComponentFactory::Instance().Create(componentType, pActor);

					if (!c) {
						ZP_EDITOR_ERROR("Component " + componentType + " is invalid !");
					}

					int index = 1;
					std::string newId;
					do
					{
						newId = componentType + std::to_string(index);
						index++;
					} while (pActor->HasComponentId(newId));

					c->SetId(newId);

					pActor->AddComponent(c);
					ZP_CORE_LOAD("Component " + componentType + " loaded and attached to " + pActor->GetName());
				}
			}
			ImGui::EndPopup();
		}

		ImGui::Separator();

		auto components = pActor->GetComponents();

		if (!components.empty())
		{
			for (int i = 0; i < components.size(); i++)
			{
				char label[32];

				sprintf(label, (components[i]->GetName() + "_%i").c_str(), i);
				ImGui::PushID(label);
				auto pos = ImGui::GetCursorPos();
				ImGui::SetCursorPos(ImVec2(pos.x + 10, pos.y + 5));
				if (ImGui::Selectable(components[i]->GetName().c_str(), selected == i, 0, ImVec2(ImGui::GetContentRegionAvail().x - 10 - 25, 0)))
				{
					selected = i;
				}
				ImGui::PopID();
				ImGui::SameLine();
				auto windowSize = ImGui::GetContentRegionAvail();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + windowSize.x - 25);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.0, 0.0, 1.0));
				if (ImGui::SmallButton(("X##" + std::to_string(i)).c_str())) // delete a specific component
				{
					//TODO Event to destroy component
					pActor->GetComponentWithId(components[i]->GetId())->OnEnd();
					pActor->RemoveComponent(pActor->GetComponentWithId(components[i]->GetId()));
					delete components[i];
					ImGui::PopStyleColor();
					break;
				}
				ImGui::PopStyleColor();
				if (ImGui::IsItemHovered())
				{
					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
					ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
					ImGui::SetTooltip(("Delete " + components[i]->GetName()).c_str());
					ImGui::PopStyleVar(2);
					ImGui::PopStyleColor();
				}
			}
		}
}

void InspectorPanel::DrawActorInfos(Actor* pActor)
{
	char buffer[64];
	strncpy(buffer, pActor->GetName().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
	ImGui::SetNextItemWidth(150.0f);

	if (ImGui::InputText("##Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		RenameActorEvent* event = new RenameActorEvent(pActor, std::string(buffer));
		EventSystem::DoEvent(event);
	}

	ImGui::SameLine();

	static bool isActive = true;

	pActor->GetState() == ActorState::Active ? isActive = true : isActive = false;

	ImGui::Text("Active");

	ImGui::SameLine();

	if (ImGui::Checkbox("##Active", &isActive))
	{
		if (isActive)
			//TODO : Set active event with undo
			pActor->SetActive(ActorState::Active);
		else
			pActor->SetActive(ActorState::Paused);
	}

	ImGui::SameLine();

	ImGui::Text("Tags");

	ImGui::SameLine();

	if (ImGui::ArrowButton("##TagArrow", ImGuiDir_Down))
	{
		ImGui::OpenPopup("TagsMenu");
	}

	ImVec2 btnPos = ImGui::GetItemRectMin();
	ImVec2 btnSize = ImGui::GetItemRectSize();

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + 15, btnPos.y + btnSize.y + 15), ImGuiCond_Always);

	if (ImGui::BeginPopup("TagsMenu", ImGuiWindowFlags_NoMove))
	{
		for (size_t i = 0; i < pActor->GetTag().size(); ++i)
		{
			ImGui::Text(pActor->GetTag()[i].c_str());
			ImGui::SameLine();
			auto windowSize = ImGui::GetContentRegionAvail();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + windowSize.x - 25);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.0, 0.0, 1.0));
			if (ImGui::SmallButton(("X##" + std::to_string(i)).c_str()))
			{
				pActor->RemoveTag(pActor->GetTag()[i]);
			}
			ImGui::PopStyleColor();
		}

		static char newTag[32] = "";
		if (ImGui::InputText("##NewTag", newTag, sizeof(newTag), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Add Tag"))
		{
			if (strlen(newTag) > 0)
			{
				pActor->AddTag(std::string(newTag));
				newTag[0] = '\0';
			}
		}

		ImGui::EndPopup();
	}

	ImGui::PopStyleVar();

	ImGui::Separator();

	float labelWidth = 70.0f;

	float position[3] = {
		pActor->GetPosition().x, pActor->GetPosition().y, pActor->GetPosition().z
	};

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Position");
	ImGui::SameLine(labelWidth);
	if (ImGui::InputFloat3("##Position", position, "%.3f"))
	{
		SetPositionEvent* posEvent = new SetPositionEvent(pActor, pActor->GetPosition(), Vector3D(position[0], position[1], position[2]));
		EventSystem::DoEvent(posEvent);
	}

	float rotation[3] = {
		pActor->GetRotationEuler().x, pActor->GetRotationEuler().y, pActor->GetRotationEuler().z
	};
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Rotation");
	ImGui::SameLine(labelWidth);
	if (ImGui::InputFloat3("##Rotation", rotation, "%.3f"))
	{
		auto euler = Vector3D(rotation[0], rotation[1], rotation[2]);

		SetRotationEvent* rotEvent = new SetRotationEvent(pActor, pActor->GetTransformComponent().GetRotation(), Quaternion(euler));
		EventSystem::DoEvent(rotEvent);
	}

	static bool keepRatio = true;
	static Vector3D originalSize;

	float size[3] = {
		pActor->GetSize().x, pActor->GetSize().y, pActor->GetSize().z
	};

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Size");
	ImGui::SameLine(labelWidth);
	if (ImGui::InputFloat3("##Size", size, "%.3f"))
	{
		Vector3D prevSize = pActor->GetSize();
		Vector3D newSize(size[0], size[1], size[2]);

		if (keepRatio)
		{
			Vector3D delta = newSize - prevSize;
			int editedAxis = 0;

			if (fabs(delta.y) > fabs(delta.x) && fabs(delta.y) > fabs(delta.z))
			{
				editedAxis = 1;
			}
			else if (fabs(delta.z) > fabs(delta.x) && fabs(delta.z) > fabs(delta.y)) 
			{
				editedAxis = 2;
			}

			float prevVal = (editedAxis == 0 ? prevSize.x : editedAxis == 1 ? prevSize.y : prevSize.z);
			float newVal = (editedAxis == 0 ? newSize.x : editedAxis == 1 ? newSize.y : newSize.z);

			float factor = (prevVal != 0.0f) ? newVal / prevVal : 1.0f;

			newSize = prevSize * factor;
		}

		SetSizeEvent* sizeEvent = new SetSizeEvent(pActor, originalSize, newSize);
		EventSystem::DoEvent(sizeEvent);

		if (keepRatio)
		{
			originalSize = newSize;
		}
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("##KeepRatio", &keepRatio))
	{
		if(keepRatio)
		{
			originalSize = pActor->GetSize();
		}
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
		ImGui::SetTooltip("Uniform Scale");
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}
}

void InspectorPanel::DrawSplitterButton(float& h)
{
	ImGui::InvisibleButton("hsplitter", ImVec2(-1, 8.0f));
	if (ImGui::IsItemActive())
	{
		h += ImGui::GetIO().MouseDelta.y;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
	}
}

void InspectorPanel::SetSceneHierarchy(SceneHierarchyPanel* pHierarchy)
{
	mHierarchy = pHierarchy;
}
