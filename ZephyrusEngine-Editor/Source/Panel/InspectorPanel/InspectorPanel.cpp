#include "InspectorPanel.h"
#include "Actor.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "EditorApplication/EventSystem/Event/SetTransformEvent.h"
#include "AssetsManager.h"
#include "Log.h"
#include "Component.h"
#include "SceneManager.h"
#include "EditorApplication/EventSystem/Event/RenameComponentEvent.h"
#include "Interface/IMesh.h"

using Zephyrus::Assets::AssetsManager;

InspectorPanel::InspectorPanel(ISceneContext* pSceneContext, const std::string& pName)
	: Panel(pSceneContext, pName)
{
	mComponentPropertyDrawer = new ComponentPropertyDrawer();
	mActorDrawer = new ActorDrawer();
}

InspectorPanel::~InspectorPanel()
{
	mHierarchy = nullptr;
	delete mComponentPropertyDrawer;
	mComponentPropertyDrawer = nullptr;
	delete mActorDrawer;
	mActorDrawer = nullptr;
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

		mActorDrawer->DrawActorInfos(actor);
		
		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::BeginChild("child2", ImVec2(0, h), true);
		ImGui::PopStyleVar();

		mActiveComponent = mActorDrawer->DrawActorComponents(actor, mContext->GetComponentFactory());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::EndChild();

		ZP::UI::DrawSplitterButton(h);

		ImGui::BeginChild("child3", ImVec2(0, 0), true);
		DrawComponentInfos(actor);
		ImGui::EndChild();
		ImGui::PopStyleVar();
	}
	else
	{
		ImGui::Text("No selected actor !");
	}
	ImGui::End();

	if (actor)
	{
		if (auto cam= GetCurrentCameraComponent())
		{
			cam->UpdateMatrices();
			cam->RenderScene();
			if (ImGui::Begin("Camera Preview")) 
			{
				ImVec2 previewSize = ImGui::GetContentRegionAvail();
				ImGui::Image((ImTextureID)(intptr_t)cam->GetRenderTarget()->GetColorTexture(), previewSize, ImVec2(0, 1), ImVec2(1, 0));
			} 
			ImGui::End();
		}
	}
	Panel::EndDraw();
}

void InspectorPanel::DrawComponentInfos(Actor* pActor)
{
	if (mActiveComponent)
	{
		char buffer[64];
		strncpy_s(buffer, mActiveComponent->GetName().c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
		ImGui::SetNextItemWidth(150.0f);

		if (ImGui::InputText("##Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			RenameComponentEvent* event = new RenameComponentEvent(mActiveComponent, std::string(buffer));
			EventSystem::DoEvent(event);
		}

		ImGui::SameLine();

		ImGui::Text("Component Tags");

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
			for (size_t i = 0; i < mActiveComponent->GetTag().size(); ++i)
			{
				ImGui::Text(mActiveComponent->GetTag()[i].c_str());
				ImGui::SameLine();
				auto windowSize = ImGui::GetContentRegionAvail();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + windowSize.x - 25);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.0, 0.0, 1.0));
				if (ImGui::SmallButton(("X##" + std::to_string(i)).c_str()))
				{
					mActiveComponent->RemoveTag(mActiveComponent->GetTag()[i]);
				}
				ImGui::PopStyleColor();
			}

			static char newTag[32] = "";
			if (ImGui::InputText("##NewTag", newTag, sizeof(newTag), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Add Tag"))
			{
				if (strlen(newTag) > 0)
				{
					mActiveComponent->AddTag(std::string(newTag));
					newTag[0] = '\0';
				}
			}

			ImGui::EndPopup();
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Relative Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{

			ImGui::AlignTextToFramePadding();

			mActorDrawer->SetComponentTransform(
				mActiveComponent,
				"Position",
				0.2f,
				mActiveComponent->GetRelativePosition(),

				[this](const Vector3D& newPos) {
					mActiveComponent->SetRelativePosition(newPos);
				},

				[this](Zephyrus::ActorComponent::Component* component, const Vector3D& oldV, const Vector3D& newV) {
					CreateSetComponentLocation(component, oldV, newV);
				}
			);

			mActorDrawer->SetComponentTransform(
				mActiveComponent,
				"Rotation",
				1.0f,
				mActiveComponent->GetRelativeRotationEuler(),

				[this](const Vector3D& newPos) {
					mActiveComponent->SetRelativeRotation(Quaternion(newPos));
				},

				[this](Zephyrus::ActorComponent::Component* component, const Vector3D& oldV, const Vector3D& newV) {
					CreateSetComponentRotation(component, oldV, newV);
				}
			);

			mActorDrawer->SetComponentTransform(
				mActiveComponent,
				"Size",
				0.1f,
				mActiveComponent->GetRelativeSize(),

				[this](const Vector3D& newPos) {

					Vector3D prevSize = mActiveComponent->GetRelativeSize();
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
					mActiveComponent->SetRelativeSize(newSize);
				},

				[this](Zephyrus::ActorComponent::Component* component, const Vector3D& oldV, const Vector3D& newV) {
					CreateSetComponentSize(component, oldV, newV);
				}
			);

			ImGui::SameLine();
			if (ImGui::Checkbox("##KeepRatioComponent", &mKeepRatio))
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

		ImGui::Separator();

		auto properties = mActiveComponent->GetProperties();
		if (!properties.empty() && ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (unsigned int i = 0; i < properties.size(); i++)
			{
				mComponentPropertyDrawer->DrawProperty(std::to_string(i), properties[i], mActiveComponent);
			}
		}
		ImGui::PopStyleVar();
	}
	else
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
		auto components = pActor->GetComponents();
		if (components.empty())
		{
			ImGui::PopStyleVar();
			return;
		}
		for (auto& component : components)
		{
			auto properties = component->GetProperties();
			ImGui::PushID(("CH_Component" + component->GetId()).c_str());
			if (!properties.empty() && ImGui::CollapsingHeader(component->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (unsigned int i = 0; i < properties.size(); i++)
				{
					std::string index = std::to_string(i) + component->GetId();
					mComponentPropertyDrawer->DrawProperty(index, properties[i], component);
				}
			}
			ImGui::PopID();
			ImGui::Dummy(ImVec2(0, 10));
		}
		ImGui::PopStyleVar();
	}
}

void InspectorPanel::SetSceneHierarchy(SceneHierarchyPanel* pHierarchy)
{
	mHierarchy = pHierarchy;
}

void InspectorPanel::CreateSetComponentLocation(Zephyrus::ActorComponent::Component* pComponent,
	const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
{
	SetComponentPositionEvent* posEvent = new SetComponentPositionEvent(pComponent, pCurrentPosition, pNextPosition);
	EventSystem::DoEvent(posEvent);
}

void InspectorPanel::CreateSetComponentRotation(Zephyrus::ActorComponent::Component* pComponent,
	const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
{
	SetComponentRotationEvent* rotEvent = new SetComponentRotationEvent(pComponent, Quaternion(pCurrentPosition), Quaternion(pNextPosition));
	EventSystem::DoEvent(rotEvent);
}

void InspectorPanel::CreateSetComponentSize(Zephyrus::ActorComponent::Component* pComponent,
	const Vector3D& pCurrentPosition, const Vector3D& pNextPosition)
{
	SetComponentSizeEvent* sizeEvent = new SetComponentSizeEvent(pComponent, pCurrentPosition, pNextPosition);
	EventSystem::DoEvent(sizeEvent);
}
