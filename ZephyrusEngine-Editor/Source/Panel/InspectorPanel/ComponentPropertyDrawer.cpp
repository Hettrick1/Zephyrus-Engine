#include "ComponentPropertyDrawer.h"

#include <filesystem>

#include "Actor.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/Event/RenameActorEvent.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "EditorApplication/EventSystem/Event/SetTransformEvent.h"
#include "AssetsManager.h"
#include "ComponentFactory.h"
#include "Log.h"
#include "Component.h"
#include "Interface/ICubeMapTexture.h"
#include "../../EditorUI/Property.h"
#include "SceneManager.h"
#include "Interface/IMesh.h"
#include "Interface/ITexture2D.h"
#include "Material/MaterialInstance.h"

ComponentPropertyDrawer::ComponentPropertyDrawer()
{
	mPropertySetters[PropertyType::Float] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyFloat(i, p, lw, iw); };
	mPropertySetters[PropertyType::Int] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyInt(i, p, lw, iw); };
	mPropertySetters[PropertyType::Bool] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyBool(i, p, lw, iw); };
	mPropertySetters[PropertyType::String] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyString(i, p, lw, iw); };
	mPropertySetters[PropertyType::Vec2] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyVector2D(i, p, lw, iw); };
	mPropertySetters[PropertyType::Vec3] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyVector3D(i, p, lw, iw); };
	mPropertySetters[PropertyType::Vec4] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyVector4D(i, p, lw, iw); };
	mPropertySetters[PropertyType::Color] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyColor(i, p, lw, iw); };
	mPropertySetters[PropertyType::Quaternion] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyQuaternion(i, p, lw, iw); };
	mPropertySetters[PropertyType::Texture2D] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyTexture(i, p, lw, iw); };
	mPropertySetters[PropertyType::Font] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyFont(i, p, lw, iw); };
	mPropertySetters[PropertyType::Mesh] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyMesh(i, p, lw, iw); };
	mPropertySetters[PropertyType::VectorTexture2D] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayTexture2D(i, p, lw, iw); };
	mPropertySetters[PropertyType::Prefab] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyPrefab(i, p, lw, iw); };
	mPropertySetters[PropertyType::CubeMap] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyCubemap(i, p, lw, iw); };
	mPropertySetters[PropertyType::Component] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyComponent(i, p, lw, iw); };
	mPropertySetters[PropertyType::MaterialInstance] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyMaterialInstance(i, p, lw, iw); };
	mPropertySetters[PropertyType::ShaderVert] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyShaderVert(i, p, lw, iw); };
	mPropertySetters[PropertyType::ShaderFrag] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyShaderFrag(i, p, lw, iw); };
	mPropertySetters[PropertyType::ShaderTesc] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyShaderTesc(i, p, lw, iw); };
	mPropertySetters[PropertyType::ShaderTese] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyShaderTese(i, p, lw, iw); };
	mPropertySetters[PropertyType::ShaderGeom] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyShaderGeom(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayFloat] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayFloat(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayInt] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayInt(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayVector2D] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayVector2D(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayVector3D] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayVector3D(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayVector4D] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayVector4D(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayTextureBase] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayTextureBase(i, p, lw, iw); };
	mPropertySetters[PropertyType::TextureBase] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyTextureBase(i, p, lw, iw); };
}

void ComponentPropertyDrawer::DrawProperty(const std::string& pIndex, const PropertyDescriptor& property, Zephyrus::ActorComponent::Component* activeComponent)
{
	mActiveComponent = activeComponent;
	ImGui::AlignTextToFramePadding();
	float labelWidth = 125;
	float inputWidth = ImGui::GetContentRegionAvail().x - labelWidth;

	bool disabled = false;
	
	auto it = mPropertySetters.find(property.type);
	if (it != mPropertySetters.end())
	{
		if (Zephyrus::PropertyFlags::HasFlag(property.metadata.flags,Zephyrus::PropertyFlags::Condition))
		{
			if (Zephyrus::PropertyFlags::HasFlag(property.metadata.flags,Zephyrus::PropertyFlags::Hidden_In_Editor) && !property.metadata.condition)
			{
				return;
			}
			if (Zephyrus::PropertyFlags::HasFlag(property.metadata.flags,Zephyrus::PropertyFlags::Disable_In_Editor) && !property.metadata.condition)
			{
				ImGui::BeginDisabled();
				disabled = true;
			}
		}
		else
		{
			if (Zephyrus::PropertyFlags::HasFlag(property.metadata.flags,(Zephyrus::PropertyFlags::Hidden_In_Editor)))
			{
				return;
			}
			if (Zephyrus::PropertyFlags::HasFlag(property.metadata.flags,Zephyrus::PropertyFlags::Disable_In_Editor))
			{
				ImGui::BeginDisabled();
				disabled = true;
			}
		}
		
		bool sep =it->second(pIndex, property, labelWidth, inputWidth);
		
		if (disabled)
		{
			ImGui::EndDisabled();
		}
		if (sep)
		{
			ImGui::Separator();
		}
	}
}

bool ComponentPropertyDrawer::SetPropertyFloat(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<float>(pProperty, mActiveComponent);
	std::string key = prop.name;
	
	float* realPtr = static_cast<float*>(prop.getter());
	float fVar = *realPtr;

	ImGui::Text(key.c_str());
	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name + pIndex;
	
	if (pProperty.metadata.maxFValue > pProperty.metadata.minFValue)
	{
		if (Zephyrus::PropertyFlags::HasFlag(pProperty.metadata.flags, Zephyrus::PropertyFlags::Range))
		{
			ImGui::SliderFloat(label.c_str(), realPtr,  pProperty.metadata.minFValue, pProperty.metadata.maxFValue);
		}
		else
		{
			ImGui::DragFloat(label.c_str(), realPtr);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		}
	}
	else
	{
		ImGui::InputFloat(label.c_str(), realPtr);
	}
	
	if (ImGui::IsItemActivated() && !mEditingFloats.contains(key))
	{
		mEditingFloats[key] = fVar;
	}
	
	if (ImGui::IsItemDeactivatedAfterEdit() && mEditingFloats.contains(key))
	{
		float oldValue = mEditingFloats[key];
		float newValue = *realPtr;

		prop.Set(&newValue, &oldValue);
		mEditingFloats.erase(key);
	}

	return true;
}

bool ComponentPropertyDrawer::SetPropertyInt(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<int>(pProperty, mActiveComponent);
	std::string key = prop.name;
	
	int* realPtr = static_cast<int*>(prop.getter());
	int iVar = *realPtr;

	ImGui::Text(key.c_str());
	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name + pIndex;
	
	if (pProperty.metadata.maxIValue > pProperty.metadata.minIValue)
	{
		if (Zephyrus::PropertyFlags::HasFlag(pProperty.metadata.flags, Zephyrus::PropertyFlags::Range))
		{
			ImGui::SliderInt(label.c_str(), realPtr,  pProperty.metadata.minIValue, pProperty.metadata.maxIValue);
		}
		else
		{
			ImGui::DragInt(label.c_str(), realPtr);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		}
	}
	else
	{
		ImGui::InputInt(label.c_str(), realPtr);
	}
	
	if (ImGui::IsItemActivated() && !mEditingInts.contains(key))
	{
		mEditingInts[key] = iVar;
	}
	
	if (ImGui::IsItemDeactivatedAfterEdit() && mEditingInts.contains(key))
	{
		int oldValue = mEditingInts[key];
		int newValue = *realPtr;

		prop.Set(&newValue, &oldValue);
		mEditingInts.erase(key);
	}

	return true;
}

bool ComponentPropertyDrawer::SetPropertyBool(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<bool>(pProperty, mActiveComponent);
	bool bVar = *static_cast<bool*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	std::string label = "##" + prop.name + pIndex;
	if (Zephyrus::PropertyFlags::HasFlag(pProperty.metadata.flags, Zephyrus::PropertyFlags::DropDown))
	{
		const char* items[]{pProperty.metadata.falseValue.c_str(), pProperty.metadata.trueValue.c_str()};
		int SelectedItem = bVar;
		if (ImGui::Combo(label.c_str(), &SelectedItem, items, IM_ARRAYSIZE(items)))
		{
			bVar = SelectedItem;
			prop.Set(&bVar);
		}
	}
	else
	{
		if (ImGui::Checkbox(label.c_str(), &bVar))
		{
			prop.Set(&bVar);
		}
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyString(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::string>(pProperty, mActiveComponent);
	std::string sVar = *static_cast<std::string*>(prop.getter());
	ImGui::Text(prop.name.c_str());

	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);

	char buffer[255];
	strncpy_s(buffer, sVar.c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';
	
	std::string label = "##String" + std::string(buffer) + pIndex;
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		prop.Set(&buffer);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyColor(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth,
	const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Vector4D>(pProperty, mActiveComponent);
	Vector4D colorVar = *static_cast<Vector4D*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	
	std::string label = "##" + prop.name + pIndex;
	float vec4[4] = { colorVar.x, colorVar.y, colorVar.z, colorVar.w };
	static float newVec4[4];
	if (ImGui::ColorButton(label.c_str(), ImVec4(vec4[0], vec4[1], vec4[2], vec4[3]), 0, ImVec2(pInputWidth, 0)))
	{
		ImGui::OpenPopup("ColorPopup");
		newVec4[0] = colorVar.x;
		newVec4[1] = colorVar.y;
		newVec4[2] = colorVar.z;
		newVec4[3] = colorVar.w;
	}
	if (ImGui::BeginPopup("ColorPopup", ImGuiWindowFlags_NoMove))
	{
		ImGui::ColorPicker4(label.c_str(), newVec4);
		if (ImGui::Button("Save Color"))
		{
			Vector4D newColorVar = Vector4D(newVec4[0], newVec4[1], newVec4[2], newVec4[3]);
			prop.Set(&newColorVar);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyVector4D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth,
	const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Vector4D>(pProperty, mActiveComponent);
	Vector4D vec4Var = *static_cast<Vector4D*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	
	std::string label = "##" + prop.name + pIndex;
	float vec4[4] = { vec4Var.x, vec4Var.y, vec4Var.z, vec4Var.w };
	if (ImGui::InputFloat4(label.c_str(), vec4, "%.3f", ImGuiInputTextFlags_AutoSelectAll))
	{
		vec4Var = Vector4D(vec4[0], vec4[1], vec4[2], vec4[3]);
		prop.Set(&vec4Var);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyVector3D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Vector3D>(pProperty, mActiveComponent);
	Vector3D vec3Var = *static_cast<Vector3D*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	
	std::string label = "##" + prop.name + pIndex;
	float vec3[3] = { vec3Var.x, vec3Var.y, vec3Var.z };
	if (ImGui::InputFloat3(label.c_str(), vec3, "%.3f", ImGuiInputTextFlags_AutoSelectAll))
	{
		vec3Var = Vector3D(vec3[0], vec3[1], vec3[2]);
		prop.Set(&vec3Var);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyVector2D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Vector2D>(pProperty, mActiveComponent);
	Vector2D vec2Var = *static_cast<Vector2D*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	
	std::string label = "##" + prop.name + pIndex;
	float vec2[2] = { vec2Var.x, vec2Var.y };
	if (ImGui::InputFloat2(label.c_str(), vec2, "%.3f", ImGuiInputTextFlags_AutoSelectAll))
	{
		vec2Var = Vector2D(vec2[0], vec2[1]);
		prop.Set(&vec2Var);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyQuaternion(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	// TODO
	return false;
}

bool ComponentPropertyDrawer::SetPropertyTexture(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	Property prop;
	prop = MakeUndoableProperty<Zephyrus::Assets::ITexture2D*>(pProperty, mActiveComponent);
	Zephyrus::Assets::ITexture2D* tex = static_cast<Zephyrus::Assets::ITexture2D*>(prop.getter());
	if (!tex)
	{
		return false;
	}
	char buffer[255];
	strncpy_s(buffer, tex->GetFilePath().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Texture : ");

	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##Texture" + std::string(buffer) + pIndex;
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::LoadTexture(buffer, buffer);
		if (newTex)
		{
			prop.Set(newTex);
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
			Zephyrus::Assets::ITexture2D* droppedTex = Zephyrus::Assets::AssetsManager::LoadTexture(textureID, textureID);
			if (droppedTex)
			{
				prop.Set(droppedTex);
			}
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(buffer);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyFont(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	// TODO
	return false;
}

bool ComponentPropertyDrawer::SetPropertyMesh(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Zephyrus::Assets::IMesh*>(pProperty, mActiveComponent);
	Zephyrus::Assets::IMesh* mesh = static_cast<Zephyrus::Assets::IMesh*>(prop.getter());
	if (!mesh)
	{
		return false;
	}
	char buffer[255];
	strncpy_s(buffer, mesh->GetFilePath().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Mesh : ");

	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##Mesh" + std::string(buffer) + pIndex;
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		Zephyrus::Assets::IMesh* newMesh = Zephyrus::Assets::AssetsManager::LoadMesh(buffer, buffer);
		if (newMesh)
		{
			prop.Set(newMesh);
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
			Zephyrus::Assets::IMesh* droppedMesh = Zephyrus::Assets::AssetsManager::LoadMesh(meshID, meshID);
			if (droppedMesh)
			{
				prop.Set(droppedMesh);
			}
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(buffer);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyCubemap(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Zephyrus::Assets::ICubeMapTexture*>(pProperty, mActiveComponent);
	Zephyrus::Assets::ICubeMapTexture* cubemap = static_cast<Zephyrus::Assets::ICubeMapTexture*>(prop.getter());

	if (!cubemap)
	{
		return false;
	}

	std::vector<std::string> newFaces = cubemap->GetTempFilePath();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 0.0, 1.0));
	ImGui::TextWrapped("All textures must be the same size and have the same format (png, jpeg, ...) ");
	ImGui::PopStyleColor();

	for (size_t i = 0; i < 6; i++)
	{
		std::string label = "Texture " + std::to_string(i);

		char buffer[128];
		strncpy_s(buffer, newFaces[i].c_str(), sizeof(buffer));
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
		Zephyrus::Assets::ICubeMapTexture* newCubemap = Zephyrus::Assets::AssetsManager::LoadCubemap(newFaces, newFaces[0]);
		if (!newCubemap)
		{
			ZP_CORE_ERROR("Cubemap creation failed!");
			return false;
		}
		cubemap->SetTempFilePath(cubemap->GetFaceFilePath());
		prop.Set(newCubemap);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyPrefab(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::string>(pProperty, mActiveComponent);
	std::string sVar = *static_cast<std::string*>(prop.getter());
	ImGui::Text(prop.name.c_str());

	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);

	char buffer[255];
	strncpy_s(buffer, sVar.c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';
	std::string label = "##PrefabName" + std::string(buffer) + pIndex;
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		prop.Set(&buffer);
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB"))
		{
			std::string prefabID((const char*)payload->Data, payload->DataSize);
			prop.Set(&prefabID);
		}
		ImGui::EndDragDropTarget();
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyComponent(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::string>(pProperty, mActiveComponent);
	std::string componentVar = *static_cast<std::string*>(prop.getter());
	ImGui::Text(prop.name.c_str());

	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);

	char buffer[255];
	strncpy_s(buffer, componentVar.c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	std::string label = "##String" + std::string(buffer) + pIndex;
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		prop.Set(&buffer);
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("COMPONENT"))
		{
			std::string componentID((const char*)payload->Data, payload->DataSize);
			prop.Set(&componentID);
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(buffer);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyArrayTexture2D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::vector<Zephyrus::Assets::ITexture2D*>>(pProperty, mActiveComponent);
	auto* textures = static_cast<std::vector<Zephyrus::Assets::ITexture2D*>*>(prop.getter());
	if (!textures)
	{
		return false;
	}
	if (ImGui::TreeNode("Textures"))
	{
		for (size_t i = 0; i < textures->size(); i++)
		{
			Zephyrus::Assets::ITexture2D* tex = (*textures)[i];
			std::string label = "Texture " + std::to_string(i);

			char buffer[128];
			if (tex)
			{
				strncpy_s(buffer, tex->GetFilePath().c_str(), sizeof(buffer));
				buffer[sizeof(buffer) - 1] = '\0';
			}
			else
			{
				buffer[0] = '\0';
			}
			ImGui::PushID((int)i);
			if (ImGui::InputText(("##" + label).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::LoadTexture(buffer, buffer);
				if (newTex)
				{
					auto newVec = *textures;
					newVec[i] = newTex;
					prop.Set(&newVec);
				}
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
				{
					std::string textureID((const char*)payload->Data, payload->DataSize);
					Zephyrus::Assets::ITexture2D* droppedTex = Zephyrus::Assets::AssetsManager::LoadTexture(textureID, textureID);
					if (droppedTex)
					{
						auto newVec = *textures;
						newVec[i] = droppedTex;
						prop.Set(&newVec);
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
					prop.Set(&newVec);

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
			prop.Set(&newVec);
		}
		ImGui::TreePop();
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyMaterialInstance(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Zephyrus::Material::MaterialInstance>(property, mActiveComponent);
	auto* instance = static_cast<Zephyrus::Material::MaterialInstance*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	char buffer[255];
	strncpy_s(buffer, instance->GetBaseMaterial()->GetFilePath().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Base Material : ");
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##String" + std::string(buffer) + pIndex;
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		auto newMaterial = Zephyrus::Assets::AssetsManager::LoadMaterial(buffer, buffer);
		if (newMaterial == instance->GetBaseMaterial())
		{
			return true;
		}

		auto materialInstance = Zephyrus::Material::MaterialInstance();
		materialInstance.SetMaterial(newMaterial);
		prop.Set(&materialInstance);
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MATERIAL"))
		{
			std::string materialID((const char*)payload->Data, payload->DataSize);
			auto droppedMat = Zephyrus::Assets::AssetsManager::LoadMaterial(materialID, materialID);
			if (droppedMat == instance->GetBaseMaterial())
			{
				return true;
			}

			auto materialInstance = Zephyrus::Material::MaterialInstance();
			materialInstance.SetMaterial(droppedMat);
			prop.Set(&materialInstance);
		}
		ImGui::EndDragDropTarget();
	}
	
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(buffer);
	}

	if (ImGui::TreeNodeEx("Material Properties", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// --- TEXTURES OVERRIDES ---

		auto texOverrides = instance->GetTextureOverrides();
		ImGui::PushID(("texture" + pIndex).c_str());
		if (!texOverrides.empty() && ImGui::TreeNode("Textures"))
		{
			for (auto& [name, tex] : texOverrides)
			{
				ImGui::PushID(name.c_str());
				ImGui::Text("%s", name.c_str());
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				if (auto* tex2D = dynamic_cast<Zephyrus::Assets::ITexture2D*>(tex))
				{
					char buffer[256];
					strncpy_s(buffer, tex2D->GetFilePath().c_str(), sizeof(buffer));
					buffer[sizeof(buffer) - 1] = '\0';

					if (ImGui::InputText(("##" + name).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					{
						auto oldTex = tex2D;
						Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::LoadTexture(buffer, buffer);
						if (newTex)
						{
							auto* evt = new SetGenericPropertyEvent<Zephyrus::Assets::ITexture2D*>(
								nullptr,
								oldTex,
								newTex,
								[instance, name](Zephyrus::Assets::ITexture2D* val) { instance->SetTexture(name, val); }
							);
							EventSystem::DoEvent(evt);
						}
					}

					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip(buffer);
					}

					// drag & drop
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
						{
							std::string texID((const char*)payload->Data, payload->DataSize);
							Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::LoadTexture(texID, texID);
							if (newTex)
							{
								auto oldTex = tex2D;
								auto* evt = new SetGenericPropertyEvent<Zephyrus::Assets::ITexture2D*>(
									nullptr,
									oldTex,
									newTex,
									[instance, name](Zephyrus::Assets::ITexture2D* val) { instance->SetTexture(name, val); }
								);
								EventSystem::DoEvent(evt);
							}
						}
						ImGui::EndDragDropTarget();
					}
				}
				else if (auto* cubemap = dynamic_cast<Zephyrus::Assets::ICubeMapTexture*>(tex))
				{
					std::vector<std::string> faces = cubemap->GetTempFilePath();
					for (size_t i = 0; i < 6; i++)
					{
						char buffer[128];
						strncpy_s(buffer, faces[i].c_str(), sizeof(buffer));
						buffer[sizeof(buffer) - 1] = '\0';

						ImGui::PushID(static_cast<int>(i));
						if (ImGui::InputText(("##Face" + std::to_string(i)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
						{
							faces[i] = buffer;
							cubemap->SetTempFilePath(faces);
						}

						if (ImGui::IsItemHovered())
						{
							ImGui::SetTooltip(buffer);
						}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
							{
								std::string texID((const char*)payload->Data, payload->DataSize);
								faces[i] = texID;
								cubemap->SetTempFilePath(faces);
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::PopID();
					}

					if (ImGui::Button(("Create Cubemap##" + name).c_str()))
					{
						Zephyrus::Assets::ICubeMapTexture* newCubemap = Zephyrus::Assets::AssetsManager::LoadCubemap(faces, faces[0]);
						if (newCubemap)
						{
							auto oldTex = cubemap;
							auto* evt = new SetGenericPropertyEvent<Zephyrus::Assets::ICubeMapTexture*>(
								nullptr,
								oldTex,
								newCubemap,
								[instance, name](Zephyrus::Assets::ICubeMapTexture* val) { instance->SetTexture(name, val); }
							);
							EventSystem::DoEvent(evt);
						}
					}
				}

				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();

		// --- FLOAT OVERRIDES ---
		auto floatOverrides = instance->GetFloatOverrides();
		ImGui::PushID(("float" + pIndex).c_str());
		if (!floatOverrides.empty() && ImGui::TreeNode("Float"))
		{
			for (auto& [name, value] : floatOverrides)
			{
				const char* floatLabel = (name + pIndex).c_str();
				ImGui::PushID(floatLabel);
				ImGui::Text("%s", name.c_str());
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				float v = value;
				ImGui::InputFloat("##Value", &v, 0.01f, 0.1f, "%.3f");
				if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					auto oldVal = value;
					auto* evt = new SetGenericPropertyEvent<float>(
						nullptr,
						oldVal,
						v,
						[instance, name](float val) { instance->SetFloat(name, val); }
					);
					EventSystem::DoEvent(evt);
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();

		// --- INT OVERRIDES ---
		auto intOverrides = instance->GetIntOverrides();
		ImGui::PushID(("int" + pIndex).c_str());
		if (!intOverrides.empty() && ImGui::TreeNode("Int"))
		{
			for (auto& [name, value] : intOverrides)
			{
				const char* intLabel = (name + pIndex).c_str();
				ImGui::PushID(intLabel);
				ImGui::Text("%s", name.c_str());
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				int v = value;
				ImGui::InputInt("##Value", &v, 0, 0);
				if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					auto oldVal = value;
					auto* evt = new SetGenericPropertyEvent<int>(
						nullptr,
						oldVal,
						v,
						[instance, name](int val) { instance->SetInt(name, val); }
					);
					EventSystem::DoEvent(evt);
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();

		// --- VECTOR2 OVERRIDES ---
		auto vec2Overrides = instance->GetVec2Overrides();
		ImGui::PushID(("vec2" + pIndex).c_str());
		if (!vec2Overrides.empty() && ImGui::TreeNode("Vector 2D"))
		{
			for (auto& [name, value] : vec2Overrides)
			{
				const char* vec2Label = (name + pIndex).c_str();
				ImGui::PushID(vec2Label);
				ImGui::Text("%s", name.c_str());
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				float v[2] = { value.x, value.y };
				ImGui::InputFloat2("##Value", v, "%.3f");
				if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					Vector2D oldVal = value;
					Vector2D newVal(v[0], v[1]);
					auto* evt = new SetGenericPropertyEvent<Vector2D>(
						nullptr,
						oldVal,
						newVal,
						[instance, name](const Vector2D& val) { instance->SetVector2D(name, val); }
					);
					EventSystem::DoEvent(evt);
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();

		// --- VECTOR3 OVERRIDES ---
		auto vec3Overrides = instance->GetVec3Overrides();
		ImGui::PushID(("vec3" + pIndex).c_str());
		if (!vec3Overrides.empty() && ImGui::TreeNode("Vector 3D"))
		{
			for (auto& [name, value] : vec3Overrides)
			{
				const char* vec3Label = (name + pIndex).c_str();
				ImGui::PushID(vec3Label);
				ImGui::Text("%s", name.c_str());
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				float v[3] = { value.x, value.y, value.z };
				ImGui::InputFloat3("##Value", v, "%.3f");
				if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					Vector3D oldVal = value;
					Vector3D newVal(v[0], v[1], v[2]);
					auto* evt = new SetGenericPropertyEvent<Vector3D>(
						nullptr,
						oldVal,
						newVal,
						[instance, name](const Vector3D& val) { instance->SetVector3D(name, val); }
					);
					EventSystem::DoEvent(evt);
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();

		// --- VECTOR4 OVERRIDES ---
		auto vec4Overrides = instance->GetVec4Overrides();
		ImGui::PushID(("vec4" + pIndex).c_str());
		if (!vec4Overrides.empty() && ImGui::TreeNode("Vector 4D"))
		{
			for (auto& [name, value] : vec4Overrides)
			{
				const char* vec4Label = (name + pIndex).c_str();
				ImGui::PushID(vec4Label);
				ImGui::Text("%s", name.c_str());
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				float v[4] = { value.x, value.y, value.z, value.w };
				ImGui::InputFloat4("##Value", v, "%.2f");
				if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
				{
					Vector4D oldVal = value;
					Vector4D newVal(v[0], v[1], v[2], v[3]);
					auto* evt = new SetGenericPropertyEvent<Vector4D>(
						nullptr,
						oldVal,
						newVal,
						[instance, name](const Vector4D& val) { instance->SetVector4D(name, val); }
					);
					EventSystem::DoEvent(evt);
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
		ImGui::TreePop();
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyShaderVert(const std::string& pIndex, const PropertyDescriptor& pProperty,
	const float& pLabelWidth, const float& pInputWidth)
{
	return SetPropertyShader(pIndex, pProperty, pLabelWidth, pInputWidth, ShaderType::VERTEX);
}

bool ComponentPropertyDrawer::SetPropertyShaderFrag(const std::string& pIndex, const PropertyDescriptor& pProperty,
	const float& pLabelWidth, const float& pInputWidth)
{
	return SetPropertyShader(pIndex, pProperty, pLabelWidth, pInputWidth, ShaderType::FRAGMENT);
}

bool ComponentPropertyDrawer::SetPropertyShaderTesc(const std::string& pIndex, const PropertyDescriptor& pProperty,
	const float& pLabelWidth, const float& pInputWidth)
{
	return SetPropertyShader(pIndex, pProperty, pLabelWidth, pInputWidth, ShaderType::TESSELLATION_CONTROL);
}

bool ComponentPropertyDrawer::SetPropertyShaderTese(const std::string& pIndex, const PropertyDescriptor& pProperty,
	const float& pLabelWidth, const float& pInputWidth)
{
	return SetPropertyShader(pIndex, pProperty, pLabelWidth, pInputWidth, ShaderType::TESSELLATION_EVALUATION);
}

bool ComponentPropertyDrawer::SetPropertyShaderGeom(const std::string& pIndex, const PropertyDescriptor& pProperty,
	const float& pLabelWidth, const float& pInputWidth)
{
	return SetPropertyShader(pIndex, pProperty, pLabelWidth, pInputWidth, ShaderType::GEOMETRY);
}

bool ComponentPropertyDrawer::SetPropertyArrayFloat(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	return false;
}

bool ComponentPropertyDrawer::SetPropertyArrayInt(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	return false;
}

bool ComponentPropertyDrawer::SetPropertyArrayVector2D(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	return false;
}

bool ComponentPropertyDrawer::SetPropertyArrayVector3D(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	return false;
}

bool ComponentPropertyDrawer::SetPropertyArrayVector4D(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	return false;
}

bool ComponentPropertyDrawer::SetPropertyArrayTextureBase(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	return false;
}

bool ComponentPropertyDrawer::SetPropertyTextureBase(const std::string& pIndex, const PropertyDescriptor& pProperty,
	const float& pLabelWidth, const float& pInputWidth)
{
	// auto prop = MakeUndoableProperty<Zephyrus::Assets::ITextureBase*>(pProperty, mActiveComponent);
	// Zephyrus::Assets::ITextureBase* texVar = static_cast<Zephyrus::Assets::ITextureBase*>(prop.getter());
	//
	// const char* items[]{"Texture 2D", "Cubemap"};
	// int SelectedItem = 0;
	// if ()
	// SelectedItem = static_cast<int>(texVar->GetType());
	// if (ImGui::Combo("Type", &SelectedItem, items, IM_ARRAYSIZE(items)))
	// {
	// 	texVar->SetType(static_cast<Zephyrus::Assets::TextureType>(SelectedItem));
	// }
	//
	// ImGui::SameLine();
	//
	// switch (texVar->GetType())
	// {
	// case Zephyrus::Assets::TextureType::Texture2D :
	// 	return SetPropertyTexture(pIndex, pProperty, pLabelWidth, pInputWidth);
	// case Zephyrus::Assets::TextureType::Cubemap :
	// 	return SetPropertyCubemap(pIndex, pProperty, pLabelWidth, pInputWidth);
	// }
	return false;
}

bool ComponentPropertyDrawer::SetPropertyShader(const std::string& pIndex, const PropertyDescriptor& pProperty,
                                                const float& pLabelWidth, const float& pInputWidth, Zephyrus::Render::ShaderType pType)
{
	Property prop;
	prop = MakeUndoableProperty<Zephyrus::Render::IShader*>(pProperty, mActiveComponent);
	Zephyrus::Render::IShader* shader = static_cast<Zephyrus::Render::IShader*>(prop.getter());
	
	char buffer[255];
	if (shader)
	{
		strncpy_s(buffer, shader->GetFilePath().c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';
	}
	else
	{
		strncpy_s(buffer, std::string("None").c_str(), sizeof(buffer));
	}
	buffer[sizeof(buffer) - 1] = '\0';
	
	ImGui::Text(prop.name.c_str());

	ImGui::SameLine(pLabelWidth * 2);

	ImGui::SetNextItemWidth(pInputWidth - pLabelWidth * 1.5f);
	std::string label = "##Texture" + std::string(buffer) + pIndex;
	
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		if (buffer[0] == '\0')
		{
			strncpy_s(buffer, std::string("None").c_str(), sizeof(buffer));
			prop.Set(nullptr);
			return true;
		}
		Zephyrus::Render::IShader* newShader = Zephyrus::Assets::AssetsManager::LoadShader(buffer, pType ,buffer);
		if (newShader)
		{
			prop.Set(newShader);
		}
		else
		{
			ZP_EDITOR_ERROR("Failed to load Shader" + std::string(buffer));
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SHADER"))
		{
			std::string shaderID((const char*)payload->Data, payload->DataSize);
			Zephyrus::Render::IShader* droppedShader = Zephyrus::Assets::AssetsManager::LoadShader(shaderID, pType, shaderID);
			if (droppedShader)
			{
				prop.Set(droppedShader);
			}
		}
		ImGui::EndDragDropTarget();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(buffer);
	}
	
	ImGui::SameLine();
	ImGui::PushID(("Clear" + pIndex).c_str());
	if (ImGui::Button("Clear"))
	{
		strncpy_s(buffer, std::string("None").c_str(), sizeof(buffer));
		prop.Set(nullptr);
		ImGui::PopID();
		return true;
	}
	ImGui::PopID();

	if (std::string(buffer) != "None" && std::filesystem::path(buffer).extension() != Zephyrus::Render::ShaderTypeToExtensionStr(pType))
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 0.0, 1.0));
		ImGui::TextWrapped("Warning : the type of the shader you selected is not the same as the one required");
		ImGui::PopStyleColor();
	}
	return true;
}
