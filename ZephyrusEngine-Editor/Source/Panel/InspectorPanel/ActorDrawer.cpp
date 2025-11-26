#include "ActorDrawer.h"
#include "Log.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/Event/RenameActorEvent.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "EditorApplication/EventSystem/Event/SetTransformEvent.h"
#include "Actor.h"
#include "ComponentFactory.h"

using Zephyrus::ActorComponent::ActorState;

void ActorDrawer::DrawActorInfos(Zephyrus::ActorComponent::Actor* pActor)
{
	char buffer[64];
	strncpy_s(buffer, pActor->GetName().c_str(), sizeof(buffer));
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

	ImGui::AlignTextToFramePadding();

	SetTransform(
		pActor,
		"Position",
		0.2f,
		pActor->GetPosition(),

		[pActor](const Vector3D& newPos) {
			pActor->SetPosition(newPos);
		},

		[this](Zephyrus::ActorComponent::Actor* actor, const Vector3D& oldV, const Vector3D& newV) {
			CreateSetLocation(actor, oldV, newV);
		}
	);

	SetTransform(
		pActor,
		"Rotation",
		1.0f,
		pActor->GetRotationEuler(),

		[pActor](const Vector3D& newPos) {
			pActor->SetRotation(Quaternion(newPos));
		},

		[this](Zephyrus::ActorComponent::Actor* actor, const Vector3D& oldV, const Vector3D& newV) {
			CreateSetRotation(actor, oldV, newV);
		}
	);

	SetTransform(
		pActor,
		"Size",
		0.1f,
		pActor->GetSize(),

		[this, pActor](const Vector3D& newPos) {

			Vector3D prevSize = pActor->GetSize();
			Vector3D newSize = newPos;

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
			pActor->SetSize(newSize);
		},

		[this](Zephyrus::ActorComponent::Actor* actor, const Vector3D& oldV, const Vector3D& newV) {
			CreateSetSize(actor, oldV, newV);
		}
	);

	ImGui::SameLine();
	if (ImGui::Checkbox("##KeepRatio", &keepRatio))
	{
		if (keepRatio)
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

void ActorDrawer::CreateSetLocation(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
{
	SetPositionEvent* posEvent = new SetPositionEvent(pActor, pCurrentPosition, pNextPosition);
	EventSystem::DoEvent(posEvent);
}

void ActorDrawer::CreateSetRotation(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
{
	SetRotationEvent* rotEvent = new SetRotationEvent(pActor, Quaternion(pCurrentPosition), Quaternion(pNextPosition));
	EventSystem::DoEvent(rotEvent);
}

void ActorDrawer::CreateSetSize(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
{
	SetSizeEvent* sizeEvent = new SetSizeEvent(pActor, pCurrentPosition, pNextPosition);
	EventSystem::DoEvent(sizeEvent);
}

void ActorDrawer::SetTransform(Zephyrus::ActorComponent::Actor* pActor, const std::string& label, float step, const Vector3D& initialValue, const std::function<void(const Vector3D&)>& realTimeSetter, const std::function<void(Zephyrus::ActorComponent::Actor*, const Vector3D&, const Vector3D&)>& eventSetter)
{
	float transform[3] = {
		initialValue.x, initialValue.y, initialValue.z
	};

	ImGui::AlignTextToFramePadding();
	ImGui::Text(label.c_str());
	ImGui::SameLine(labelWidth);
	if (ImGui::DragFloat3(("##" + label).c_str(), transform, step, 0.0f, 0.0f, "%.3f"))
	{
		realTimeSetter(Vector3D(transform[0], transform[1], transform[2]));
	}
	if (ImGui::IsItemActivated())
	{
		oldActorTransform = Vector3D(transform[0], transform[1], transform[2]);
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
	}
	if (ImGui::IsItemDeactivatedAfterEdit() && !ImGui::IsItemActive())
	{
		eventSetter(pActor, oldActorTransform, Vector3D(transform[0], transform[1], transform[2]));
	}
}

Zephyrus::ActorComponent::Component* ActorDrawer::DrawActorComponents(Zephyrus::ActorComponent::Actor* pActor, Zephyrus::Factory::ComponentFactory* compFactory)
{
	ImGui::PushFont(ZP::UI::gFonts.medium);
	ImGui::Text("Components");
	ImGui::PopFont();

	ImGui::SameLine();

	ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x);

	if (ImGui::Button("+ Add Component"))
	{
		ImGui::OpenPopup("AddCompMenu");
	}

	if (ImGui::BeginPopup("AddCompMenu", ImGuiWindowFlags_NoMove))
	{
		for (auto& componentType : compFactory->GetComponentNames())
		{
			if (componentType == "SkySphereComponent") 
			{
				continue;
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::Button(componentType.c_str()))
			{
				Component* c = compFactory->Create(componentType, pActor);

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

				c->OnStart();

				pActor->AddComponent(c);
				ZP_EDITOR_LOAD("Component " + componentType + " loaded and attached to " + pActor->GetName());
			}
			ImGui::PopStyleColor();
		}
		ImGui::EndPopup();
	}

	ImGui::Separator();

	auto components = pActor->GetComponents();

	if (!components.empty())
	{
		ImGuiTreeNodeFlags selfFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_LabelSpanAllColumns | ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_Leaf;
		if (ImGui::TreeNodeEx("Self", selfFlags))
		{
			for (size_t i = 0; i < components.size(); i++)
			{
				if (selected >= components.size())
				{
					selected = 0;
				}

				char label[32];

				sprintf_s(label, (components[i]->GetName() + "_%i").c_str(), i);
				ImGui::PushID(label);
				auto pos = ImGui::GetCursorPos();
				ImGui::SetCursorPos(ImVec2(pos.x + 10, pos.y + 5));
				
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_LabelSpanAllColumns | ImGuiTreeNodeFlags_OpenOnArrow;
				if (selected == i)
				{
					flags |= ImGuiTreeNodeFlags_Selected;
				}
				ImGui::SetNextItemWidth(50.0f);
				if (ImGui::TreeNodeEx(components[i]->GetName().c_str(), flags))
				{
					ImGui::TreePop();
				}
				if (ImGui::IsItemClicked())
				{
					selected = i;
				}
				// if (ImGui::Selectable(components[i]->GetName().c_str(), selected == i, 0, ImVec2(ImGui::GetContentRegionAvail().x - 10 - 25, 0)))
				// {
				// 	selected = i;
				// }
				activeComponent = components[selected];
				ImGui::PopID();
				ImGui::SameLine();

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					ImGui::SetDragDropPayload("COMPONENT", components[i]->GetId().c_str(), components[i]->GetId().size());
					ImGui::Text(components[i]->GetId().c_str());
					ImGui::EndDragDropSource();
				}

				auto windowSize = ImGui::GetContentRegionAvail();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + windowSize.x - 25);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.0, 0.0, 1.0));
				bool pressed = ImGui::SmallButton(("X##" + std::to_string(i)).c_str()); // delete a specific component
				ImGui::PopStyleColor();
				if (pressed)
				{
					//TODO Event to destroy component
					pActor->GetComponentWithId(components[i]->GetId())->OnEnd();
					pActor->RemoveComponent(pActor->GetComponentWithId(components[i]->GetId()));
					delete components[i];
					std::erase(components, components[i]);
					int newSelected = pActor->GetComponents().size() - 1;
					selected = newSelected >= 0 ? newSelected : 0; 
					if (activeComponent != components[selected])
					{
						activeComponent = components[selected];
						ImGui::TreePop();
						return activeComponent;
					}
					break;
				}
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
			ImGui::TreePop();
			return activeComponent;
		}
		return nullptr;
	}
}
