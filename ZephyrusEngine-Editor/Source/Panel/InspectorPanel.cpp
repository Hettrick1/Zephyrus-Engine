#include "InspectorPanel.h"
#include "Actor.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/Event/RenameActorEvent.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "EditorApplication/EventSystem/Event/SetTransformEvent.h"
#include "Assets.h"
#include "ComponentFactory.h"
#include "Log.h"
#include "Component.h"
#include "CubeTextureMap.h"
#include "../EditorUI/Property.h"
#include "SceneManager.h"
#include "Interface/IMesh.h"

using Zephyrus::Assets::AssetsManager;

InspectorPanel::InspectorPanel(ISceneContext* pSceneContext, const std::string& pName)
	: Panel(pSceneContext, pName)
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
		DrawComponentInfos();
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
		for (auto componentType : mContext->GetComponentFactory()->GetComponentNames())
		{
			if (componentType == "SkySphereComponent") 
			{
				continue;
			}
			if (ImGui::Button(componentType.c_str()))
			{
				Component* c = mContext->GetComponentFactory()->Create(componentType, pActor);

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
		}
		ImGui::EndPopup();
	}

	ImGui::Separator();

	auto components = pActor->GetComponents();

	if (!components.empty())
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (selected >= components.size())
			{
				selected = 0;
			}

			char label[32];

			sprintf(label, (components[i]->GetName() + "_%i").c_str(), i);
			ImGui::PushID(label);
			auto pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(pos.x + 10, pos.y + 5));
			if (ImGui::Selectable(components[i]->GetName().c_str(), selected == i, 0, ImVec2(ImGui::GetContentRegionAvail().x - 10 - 25, 0)))
			{
				selected = i;
			}
			mActiveComponent = components[selected];
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
				selected = pActor->GetComponents().size() - 1;
				if (selected < 0)
				{
					mActiveComponent = nullptr;
					selected = 0;
					return;
				}
				if (mActiveComponent != components[selected])
				{
					mActiveComponent = components[selected];
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

void InspectorPanel::DrawComponentInfos()
{
	if (mActiveComponent)
	{
		char buffer[64];
		strncpy(buffer, mActiveComponent->GetName().c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
		ImGui::SetNextItemWidth(150.0f);

		if (ImGui::InputText("##Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			/*RenameActorEvent* event = new RenameActorEvent(mActiveComponent, std::string(buffer));
			EventSystem::DoEvent(event);*/
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
			float labelWidth = 70.0f;

			float position[3] = {
				mActiveComponent->GetRelativePosition().x, mActiveComponent->GetRelativePosition().y, mActiveComponent->GetRelativePosition().z
			};

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Position");
			ImGui::SameLine(labelWidth);
			if (ImGui::InputFloat3("##RelativePosition", position, "%.3f"))
			{
				//TODO Event to set relative pos
				auto pos = Vector3D(position[0], position[1], position[2]);
				mActiveComponent->SetRelativePosition(pos);
			}

			float rotation[3] = {
				mActiveComponent->GetRelativeRotationEuler().x, mActiveComponent->GetRelativeRotationEuler().y, mActiveComponent->GetRelativeRotationEuler().z
			};
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Rotation");
			ImGui::SameLine(labelWidth);
			if (ImGui::InputFloat3("##RelativeRotation", rotation, "%.3f"))
			{
				auto euler = Vector3D(rotation[0], rotation[1], rotation[2]);

				//TODO Event to set relative rot
				mActiveComponent->SetRelativeRotation(Quaternion(euler));
			}

			static bool keepRatio = true;
			static Vector3D originalSize;

			float size[3] = {
				mActiveComponent->GetRelativeSize().x, mActiveComponent->GetRelativeSize().y, mActiveComponent->GetRelativeSize().z
			};

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Size");
			ImGui::SameLine(labelWidth);
			if (ImGui::InputFloat3("##RelativeSize", size, "%.3f"))
			{
				Vector3D prevSize = mActiveComponent->GetRelativeSize();
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

				//TODO Event to set relative size
				mActiveComponent->SetRelativeSize(newSize);

				if (keepRatio)
				{
					originalSize = newSize;
				}
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("##RelativeKeepRatio", &keepRatio))
			{
				if (keepRatio)
				{
					originalSize = mActiveComponent->GetRelativeSize();
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
			for (auto property : properties)
			{
				DrawProperty(property);
			}
		}
		ImGui::PopStyleVar();
	}
}

void InspectorPanel::DrawProperty(const PropertyDescriptor& property)
{
	ImGui::AlignTextToFramePadding();
	float labelWidth = 125;
	float inputWidth = ImGui::GetContentRegionAvail().x - labelWidth;
	switch (property.type)
	{
	case PropertyType::Float:
		SetPropertyFloat(property, labelWidth, inputWidth);
		break;
	case PropertyType::Int:
		SetPropertyInt(property, labelWidth, inputWidth);
		break;
	case PropertyType::Bool:
		SetPropertyBool(property, labelWidth, inputWidth);
		break;
	case PropertyType::String:
		SetPropertyString(property, labelWidth, inputWidth);
		break;
	case PropertyType::Vec3:
		SetPropertyVector3D(property, labelWidth, inputWidth);
		break;
	case PropertyType::Vec2:
		SetPropertyVector2D(property, labelWidth, inputWidth);
		break;
	case PropertyType::Quaternion:
		SetPropertyQuaternion(property, labelWidth, inputWidth);
		break;
	case PropertyType::Texture:
		SetPropertyTexture(property, labelWidth, inputWidth);
		break;
	case PropertyType::Font:
		SetPropertyFont(property, labelWidth, inputWidth);
		break;
	case PropertyType::Mesh:
		SetPropertyMesh(property, labelWidth, inputWidth);
		break;
	case PropertyType::VectorTexture:
		SetPropertyVectorTexture(property, labelWidth, inputWidth);
		break;
	case PropertyType::Prefab:
		SetPropertyPrefab(property, labelWidth, inputWidth);
		break;
	case PropertyType::CubeMap:
		SetPropertyCubemap(property, labelWidth, inputWidth);
		break;
	case PropertyType::Component:
		SetPropertyComponent(property, labelWidth, inputWidth);
		break;
	default:
		break;
	}
	ImGui::Separator();
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

void InspectorPanel::SetPropertyFloat(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<float>(pProperty, mActiveComponent);
	float fVar = *static_cast<float*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name;
	if (ImGui::InputFloat(label.c_str(), &fVar))
	{
		prop.setter(&fVar);
	}
}

void InspectorPanel::SetPropertyInt(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<int>(pProperty, mActiveComponent);
	int iVar = *static_cast<int*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name;
	if (ImGui::InputInt(label.c_str(), &iVar))
	{
		prop.setter(&iVar);
	}
}

void InspectorPanel::SetPropertyBool(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<bool>(pProperty, mActiveComponent);
	bool bVar = *static_cast<bool*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	std::string label = "##" + prop.name;
	if (ImGui::Checkbox(label.c_str(), &bVar))
	{
		prop.setter(&bVar);
	}
}

void InspectorPanel::SetPropertyString(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::string>(pProperty, mActiveComponent);
	std::string sVar = *static_cast<std::string*>(prop.getter());
	ImGui::Text(prop.name.c_str());

	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);

	char buffer[255];
	strncpy(buffer, sVar.c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	if (ImGui::InputText(("##String" + std::string(buffer)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		prop.setter(&buffer);
	}
}

void InspectorPanel::SetPropertyVector3D(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Vector3D>(pProperty, mActiveComponent);
	Vector3D vec3Var = *static_cast<Vector3D*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name;
	float vec3[3] = { vec3Var.x, vec3Var.y, vec3Var.z };
	if (ImGui::InputFloat3(label.c_str(), vec3, "%.3f", ImGuiInputTextFlags_AutoSelectAll))
	{
		vec3Var = Vector3D(vec3[0], vec3[1], vec3[2]);
		prop.setter(&vec3Var);
	}
}

void InspectorPanel::SetPropertyVector2D(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Vector2D>(pProperty, mActiveComponent);
	Vector2D vec2Var = *static_cast<Vector2D*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name;
	float vec2[2] = { vec2Var.x, vec2Var.y };
	if (ImGui::InputFloat2(label.c_str(), vec2, "%.3f", ImGuiInputTextFlags_AutoSelectAll))
	{
		vec2Var = Vector2D(vec2[0], vec2[1]);
		prop.setter(&vec2Var);
	}
}

void InspectorPanel::SetPropertyQuaternion(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
}

void InspectorPanel::SetPropertyTexture(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	Property prop;
	if (pProperty.isPointer)
	{
		prop = MakeUndoableProperty<Texture*>(pProperty, mActiveComponent);
	}
	else
	{
		prop = MakeUndoableProperty<Texture>(pProperty, mActiveComponent);
	}
	Zephyrus::Assets::Texture* tex = static_cast<Zephyrus::Assets::Texture*>(prop.getter());
	if (!tex)
	{
		return;
	}
	char buffer[255];
	strncpy(buffer, tex->GetTextureFilePath().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Texture : ");

	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	if (ImGui::InputText(("##Texture" + std::string(buffer)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		Zephyrus::Assets::Texture* newTex = AssetsManager::LoadTexture(buffer, buffer);
		if (newTex)
		{
			prop.setter(newTex);
		}
		else
		{
			ZP_EDITOR_ERROR("Failed to load Texture " + std::string(buffer));
		}
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
		{
			std::string textureID((const char*)payload->Data, payload->DataSize);
			Zephyrus::Assets::Texture* droppedTex = AssetsManager::LoadTexture(textureID, textureID);
			if (droppedTex)
			{
				prop.setter(droppedTex);
			}
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(buffer);
	}
}

void InspectorPanel::SetPropertyFont(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
}

void InspectorPanel::SetPropertyMesh(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Zephyrus::Assets::IMesh*>(pProperty, mActiveComponent);
	Zephyrus::Assets::IMesh* mesh = static_cast<Zephyrus::Assets::IMesh*>(prop.getter());
	if (!mesh)
	{
		return;
	}
	char buffer[255];
	strncpy(buffer, mesh->GetFilePath().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Mesh : ");

	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	if (ImGui::InputText(("##Mesh" + std::string(buffer)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		Zephyrus::Assets::IMesh* newMesh = AssetsManager::LoadMesh(buffer, buffer);
		if (newMesh)
		{
			prop.setter(newMesh);
		}
		else
		{
			ZP_EDITOR_ERROR("Failed to load Mesh " + std::string(buffer));
		}
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH"))
		{
			std::string meshID((const char*)payload->Data, payload->DataSize);
			Zephyrus::Assets::IMesh* droppedMesh = AssetsManager::LoadMesh(meshID, meshID);
			if (droppedMesh)
			{
				prop.setter(droppedMesh);
			}
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(buffer);
	}
}

void InspectorPanel::SetPropertyCubemap(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Zephyrus::Assets::CubeTextureMap>(pProperty, mActiveComponent);
	Zephyrus::Assets::CubeTextureMap* cubemap = static_cast<Zephyrus::Assets::CubeTextureMap*>(prop.getter());

	if (!cubemap)
	{
		return;
	}

	std::vector<std::string> newFaces = cubemap->GetTempFilePath();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 0.0, 1.0));
	ImGui::TextWrapped("All textures must be the same size and have the same format (png, jpeg, ...) ");
	ImGui::PopStyleColor();

	for (size_t i = 0; i < 6; i++)
	{
		std::string label = "Texture " + std::to_string(i);

		char buffer[128];
		strncpy(buffer, newFaces[i].c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		ImGui::PushID((int)i);
		if (ImGui::InputText(("##" + label).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			newFaces[i] = buffer;
			cubemap->SetTempFilePath(newFaces);
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
			{
				std::string textureID((const char*)payload->Data, payload->DataSize);
				newFaces[i] = textureID;
				cubemap->SetTempFilePath(newFaces);
			}
			ImGui::EndDragDropTarget();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(buffer);
		}
		ImGui::PopID();
	}
	if (ImGui::Button("Create Texture Map"))
	{
		Zephyrus::Assets::CubeTextureMap newCubemap;
		if (newCubemap.CreateCubeTextureMap(newFaces))
		{
			// reset temp files for old cubetexturemap
			cubemap->SetTempFilePath(cubemap->GetFaceFilePath());
			prop.setter(&newCubemap);
		}
		else
		{
			ZP_EDITOR_ERROR("cannot create the cube texture map");
		}
	}
}

void InspectorPanel::SetPropertyPrefab(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::string>(pProperty, mActiveComponent);
	std::string sVar = *static_cast<std::string*>(prop.getter());
	ImGui::Text(prop.name.c_str());

	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);

	char buffer[255];
	strncpy(buffer, sVar.c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	if (ImGui::InputText(("##PrefabName" + std::string(buffer)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		prop.setter(&buffer);
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB"))
		{
			std::string meshID((const char*)payload->Data, payload->DataSize);
			prop.setter(&meshID);
		}
		ImGui::EndDragDropTarget();
	}
}

void InspectorPanel::SetPropertyComponent(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::string>(pProperty, mActiveComponent);
	std::string componentVar = *static_cast<std::string*>(prop.getter());
	ImGui::Text(prop.name.c_str());

	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);

	char buffer[255];
	strncpy(buffer, componentVar.c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	static int index = 0;

	if (ImGui::InputText(("##String" + std::string(buffer)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		prop.setter(&buffer);
		index++;
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("COMPONENT"))
		{
			std::string componentID((const char*)payload->Data, payload->DataSize);
			prop.setter(&componentID);
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(buffer);
	}
}

void InspectorPanel::SetPropertyVectorTexture(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::vector<Zephyrus::Assets::Texture*>>(pProperty, mActiveComponent);
	auto* textures = static_cast<std::vector<Zephyrus::Assets::Texture*>*>(prop.getter());
	if (!textures)
	{
		return;
	}
	if (ImGui::TreeNode("Textures"))
	{
		for (size_t i = 0; i < textures->size(); i++)
		{
			Zephyrus::Assets::Texture* tex = (*textures)[i];
			std::string label = "Texture " + std::to_string(i);

			char buffer[128];
			if (tex)
			{
				strncpy(buffer, tex->GetTextureFilePath().c_str(), sizeof(buffer));
				buffer[sizeof(buffer) - 1] = '\0';
			}
			else
			{
				buffer[0] = '\0';
			}
			ImGui::PushID((int)i);
			if (ImGui::InputText(("##" + label).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				Zephyrus::Assets::Texture* newTex = AssetsManager::LoadTexture(buffer, buffer);
				if (newTex)
				{
					auto newVec = *textures;
					newVec[i] = newTex;
					prop.setter(&newVec);
				}
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
				{
					std::string textureID((const char*)payload->Data, payload->DataSize);
					Zephyrus::Assets::Texture* droppedTex = AssetsManager::LoadTexture(textureID, textureID);
					if (droppedTex)
					{
						auto newVec = *textures;
						newVec[i] = droppedTex;
						prop.setter(&newVec);
					}
				}
				ImGui::EndDragDropTarget();
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(buffer);
			}
			if (textures->size() > 1)
			{
				ImGui::SameLine();
				if (ImGui::Button("Remove"))
				{
					auto newVec = *textures;
					newVec.erase(newVec.begin() + i);
					prop.setter(&newVec);

					ImGui::PopID();
					break;
				}
			}
			ImGui::PopID();
		}
		if (ImGui::Button("+ Add Texture"))
		{
			auto newVec = *textures;
			newVec.push_back(nullptr);
			prop.setter(&newVec);
		}
		ImGui::TreePop();
	}
}
