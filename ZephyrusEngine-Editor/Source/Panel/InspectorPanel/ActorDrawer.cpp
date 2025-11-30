#include "pch.h"
#include "ActorDrawer.h"
#include "Log.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/Event/RenameActorEvent.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "EditorApplication/EventSystem/Event/SetTransformEvent.h"
#include "Actor.h"
#include "ComponentFactory.h"
#include "EditorApplication/EventSystem/Event/RenameComponentEvent.h"

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

	SetActorTransform(
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

	SetActorTransform(
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

	SetActorTransform(
		pActor,
		"Size",
		0.1f,
		pActor->GetSize(),

		[this, pActor](const Vector3D& newPos) {

			Vector3D prevSize = pActor->GetSize();
			Vector3D newSize = newPos;

			if (mKeepRatio)
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
	if (ImGui::Checkbox("##KeepRatio", &mKeepRatio))
	{
		if (mKeepRatio)
		{
			mOriginalSize = pActor->GetSize();
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
	SetActorPositionEvent* posEvent = new SetActorPositionEvent(pActor, pCurrentPosition, pNextPosition);
	EventSystem::DoEvent(posEvent);
}

void ActorDrawer::CreateSetRotation(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
{
	SetActorRotationEvent* rotEvent = new SetActorRotationEvent(pActor, Quaternion(pCurrentPosition), Quaternion(pNextPosition));
	EventSystem::DoEvent(rotEvent);
}

void ActorDrawer::CreateSetSize(Zephyrus::ActorComponent::Actor* pActor, const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
{
	SetActorSizeEvent* sizeEvent = new SetActorSizeEvent(pActor, pCurrentPosition, pNextPosition);
	EventSystem::DoEvent(sizeEvent);
}

void ActorDrawer::SetActorTransform(Zephyrus::ActorComponent::Actor* pActor, const std::string& label, float step, const Vector3D& initialValue, const std::function<void(const Vector3D&)>& realTimeSetter, const std::function<void(Zephyrus::ActorComponent::Actor*, const Vector3D&, const Vector3D&)>& eventSetter)
{
	float transform[3] = {
		initialValue.x, initialValue.y, initialValue.z
	};

	ImGui::AlignTextToFramePadding();
	ImGui::Text(label.c_str());
	ImGui::SameLine(mLabelWidth);
	if (ImGui::DragFloat3(("##" + label).c_str(), transform, step, 0.0f, 0.0f, "%.3f"))
	{
		realTimeSetter(Vector3D(transform[0], transform[1], transform[2]));
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = pActor->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
	if (ImGui::IsItemActivated())
	{
		mOldActorTransform = Vector3D(transform[0], transform[1], transform[2]);
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
	}
	if (ImGui::IsItemDeactivatedAfterEdit() && !ImGui::IsItemActive())
	{
		eventSetter(pActor, mOldActorTransform, Vector3D(transform[0], transform[1], transform[2]));
	}
}

void ActorDrawer::SetComponentTransform(Zephyrus::ActorComponent::Component* pComponent, const std::string& label,
	float step, const Vector3D& initialValue, const std::function<void(const Vector3D&)>& realTimeSetter,
	const std::function<void(Zephyrus::ActorComponent::Component*, const Vector3D&, const Vector3D&)>& eventSetter)
{
	float transform[3] = {
		initialValue.x, initialValue.y, initialValue.z
	};

	ImGui::AlignTextToFramePadding();
	ImGui::Text(label.c_str());
	ImGui::SameLine(mLabelWidth);
	if (ImGui::DragFloat3(("##" + label).c_str(), transform, step, 0.0f, 0.0f, "%.3f"))
	{
		realTimeSetter(Vector3D(transform[0], transform[1], transform[2]));
		std::vector<Zephyrus::ActorComponent::BulletColliderComponent*> colliders = pComponent->GetOwner()->GetAllComponentOfType<Zephyrus::ActorComponent::BulletColliderComponent>();
		if (!colliders.empty())
		{
			for (auto col : colliders)
			{
				col->UpdateWorldTransform();
			}
		}
	}
	if (ImGui::IsItemActivated())
	{
		mOldActorTransform = Vector3D(transform[0], transform[1], transform[2]);
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
	}
	if (ImGui::IsItemDeactivatedAfterEdit() && !ImGui::IsItemActive())
	{
		eventSetter(pComponent, mOldActorTransform, Vector3D(transform[0], transform[1], transform[2]));
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
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleColor();
		}
		ImGui::EndPopup();
	}

	ImGui::Separator();

	auto components = pActor->GetComponents();
	ImGuiTreeNodeFlags selfFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_Leaf;
	if (mSelfSelected)
	{
		selfFlags |= ImGuiTreeNodeFlags_Selected;
	}
	bool selfTree = ImGui::TreeNodeEx("Root", selfFlags);

	if (ImGui::IsItemClicked())
	{
		mSelfSelected = true;
		mActiveComponent = nullptr;
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("COMPONENT"))
		{
			std::string componentID((const char*)payload->Data, payload->DataSize);
			if (auto draggedComp = pActor->GetComponentWithId(componentID))
			{
				draggedComp->SetParent(nullptr);
			}
		}
		ImGui::EndDragDropTarget();
	}
	
	if (!components.empty())
	{
		for (Component* comp : pActor->GetComponents())
		{
			if (comp->GetParent() == nullptr)
			{
				if (DrawComponentTree(comp))
				{
					DeleteComponentRootAndChildren(pActor, comp);
					break;
				}
			}
		}
	}

	if (selfTree)
	{
		ImGui::TreePop();
	}
	
	if (mCurrentActor != pActor)
	{
		mActiveComponent = nullptr;
		mSelfSelected = true;
		mCurrentActor = pActor;
	}
	
	return mActiveComponent;
}

bool ActorDrawer::DrawComponentTree(Zephyrus::ActorComponent::Component* pComponent)
{	
	ImGuiTreeNodeFlags flags =
		ImGuiTreeNodeFlags_FramePadding |
		ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_DefaultOpen;

	if (pComponent == mActiveComponent)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (pComponent->GetChildren().empty())
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
	
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 40);
	float backup_work_max_x = window->WorkRect.Max.x;
	window->WorkRect.Max.x = window->DC.CursorPos.x + ImGui::CalcItemWidth();

	ImGui::PushID(("CH_Component" + pComponent->GetId()).c_str());
	bool opened = ImGui::TreeNodeEx(pComponent->GetName().c_str(), flags);
	ImGui::PopID();
	
	window->WorkRect.Max.x = backup_work_max_x;
	
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		mSelfSelected = false;
		mActiveComponent = pComponent;
	}

	float width = ImGui::GetItemRectSize().x;
	if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		if (pComponent)
		{
			ImGui::OpenPopup("RenameComponent");
		}
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImGui::GetItemRectMin(), ImGuiCond_Always);

	if (ImGui::BeginPopup("RenameComponent", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{
		const char* renameLabel = "##RenameComponent";
		
		char buffer[64];
		strncpy_s(buffer, pComponent->GetName().c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		ImGui::SetNextItemWidth(width);
		ImGui::PushStyleColor(ImGuiCol_FrameBg,        IM_COL32(229, 178, 0, 150));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(66, 150, 250, 255));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  IM_COL32(66, 150, 250, 255));
		
		if (ImGui::IsWindowAppearing())
		{
			ImGui::SetKeyboardFocusHere();
		}
		
		if (ImGui::InputText(renameLabel, buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			RenameComponentEvent* event = new RenameComponentEvent(pComponent, std::string(buffer));
			EventSystem::DoEvent(event);
		}

		if (ImGui::IsItemDeactivated())
		{
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::PopStyleColor(3);
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
	
	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("COMPONENT", pComponent->GetId().c_str(), pComponent->GetId().size());
		ImGui::Text("%s", pComponent->GetName().c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("COMPONENT"))
		{
			std::string componentID((const char*)payload->Data, payload->DataSize);
			if (auto draggedComp = pComponent->GetOwner()->GetComponentWithId(componentID))
			{
				if (pComponent->GetParent() != draggedComp)
				{
					if (draggedComp->GetParent() == pComponent)
					{
						pComponent->RemoveChild(draggedComp);
					}
					else
					{
						pComponent->AddChild(draggedComp);
					}
				}
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	
	auto windowSize = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + windowSize.x - 25);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.0, 0.0, 1.0));
	bool deleteMe = ImGui::SmallButton(("X##BTN_DeleteComp" + pComponent->GetId()).c_str());
	ImGui::PopStyleColor();
	
	if (opened)
	{
		if (!pComponent->GetChildren().empty())
		{
			for (auto* child : pComponent->GetChildren())
			{
				if (DrawComponentTree(child))
				{
					DeleteComponentRootAndChildren(child->GetOwner(), child);
					break;
				}
			}
		}
		ImGui::TreePop();
	}

	return deleteMe;
}

void ActorDrawer::DeleteComponentRootAndChildren(Actor* actor, Component* comp)
{
	auto children = comp->GetChildren();
	for (auto child : children)
	{
		DeleteComponentRootAndChildren(actor, child);
	}
	
	comp->OnEnd();
	if (comp->GetParent())
	{
		comp->GetParent()->RemoveChild(comp);
	}
	actor->RemoveComponent(comp);

	if (mActiveComponent == comp)
	{
		mSelfSelected = true;
		mActiveComponent = nullptr;
	}

	delete comp;
	comp = nullptr;
}