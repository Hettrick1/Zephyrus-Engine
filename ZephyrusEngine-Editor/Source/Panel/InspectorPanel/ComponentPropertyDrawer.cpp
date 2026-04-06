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
#include "EngineContentIds.h"
#include "Interface/ICubeMapTexture.h"
#include "../../EditorUI/Property.h"
#include "SceneManager.h"
#include "Interface/IMesh.h"
#include "Interface/ITexture2D.h"
#include "Material/MaterialInstance.h"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

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
	mPropertySetters[PropertyType::ArrayMatFloat] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayFloat(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayMatInt] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayInt(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayMatVector2D] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayVector2D(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayMatVector3D] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayVector3D(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayMatVector4D] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayVector4D(i, p, lw, iw); };
	mPropertySetters[PropertyType::ArrayMatTextureBase] = [this](const std::string& i, const PropertyDescriptor& p, float lw, float iw) { return SetPropertyArrayTextureBase(i, p, lw, iw); };
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
	strncpy_s(buffer, tex->GetTextureFileId().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Texture : ");

	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##Texture" + std::string(buffer) + pIndex;
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(buffer, buffer);
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
			Zephyrus::Assets::ITexture2D* droppedTex = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(textureID);
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
	strncpy_s(buffer, mesh->GetFileId().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Mesh : ");

	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##Mesh" + std::string(buffer) + pIndex;
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		Zephyrus::Assets::IMesh* newMesh = Zephyrus::Assets::AssetsManager::GetInstance().LoadMesh(buffer);
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
			Zephyrus::Assets::IMesh* droppedMesh = Zephyrus::Assets::AssetsManager::GetInstance().LoadMesh(meshID);
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

	std::vector<std::string> newFaces = cubemap->GetTempFileIds();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 0.0, 1.0));
	ImGui::TextWrapped("All textures must be the same size and have the same format (png, jpeg, ...) ");
	ImGui::PopStyleColor();

	for (size_t i = 0; i < 6; i++)
	{
		std::string label = "Texture " + std::to_string(i);

		char buffer[128];
		strncpy_s(buffer, newFaces[i].c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		//ImGui::SetCursorPosX(pLabelWidth);

		ImGui::PushID((int)i);
		if (ImGui::InputText(("##" + label).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			newFaces[i] = buffer;
			cubemap->SetTempFileIds(newFaces);
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
			{
				std::string textureID((const char*)payload->Data, payload->DataSize);
				newFaces[i] = textureID;
				cubemap->SetTempFileIds(newFaces);
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
		std::string name;
		for (auto face : newFaces)
		{
			name += face;
		}
		Zephyrus::Assets::ICubeMapTexture* newCubemap = Zephyrus::Assets::AssetsManager::GetInstance().LoadCubemap(newFaces, name);
		if (!newCubemap)
		{
			ZP_CORE_ERROR("Cubemap creation failed!");
			return false;
		}
		cubemap->SetTempFileIds(cubemap->GetFaceFileIds());
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
				strncpy_s(buffer, tex->GetTextureFileId().c_str(), sizeof(buffer));
				buffer[sizeof(buffer) - 1] = '\0';
			}
			else
			{
				buffer[0] = '\0';
			}
			ImGui::PushID((int)i);
			if (ImGui::InputText(("##" + label).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(buffer, buffer);
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
					Zephyrus::Assets::ITexture2D* droppedTex = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(textureID);
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
					newVec.erase(newVec.begin() + static_cast<size_t>(i));
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
	std::string label = "##StringInst" + std::string(buffer) + pIndex;
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		auto newMaterial = Zephyrus::Assets::AssetsManager::GetInstance().LoadMaterial(buffer, buffer);
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
			auto droppedMat = Zephyrus::Assets::AssetsManager::GetInstance().LoadMaterial(materialID);
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

	if (ImGui::TreeNodeEx("Material Properties ##Inst", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// --- TEXTURES OVERRIDES ---
		auto texOverrides = instance->GetTextureOverrides();
		ImGui::PushID(("textureInst" + pIndex).c_str());
		if (!texOverrides.empty() && ImGui::TreeNode("Textures##Inst"))
		{
			for (auto& [name, tex] : texOverrides)
			{
				std::string texLabel = "TexInst" + name;
				ImGui::PushID(texLabel.c_str());
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));
				ImGui::Text("%s", name.c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				if (auto* tex2D = dynamic_cast<Zephyrus::Assets::ITexture2D*>(tex))
				{
					char instanceTextureBuffer[256];
					strncpy_s(instanceTextureBuffer, tex2D->GetTextureFileId().c_str(), sizeof(instanceTextureBuffer));
					instanceTextureBuffer[sizeof(instanceTextureBuffer) - 1] = '\0';

					if (ImGui::InputText(("##Inst" + name).c_str(), instanceTextureBuffer, sizeof(instanceTextureBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					{
						auto oldTex = tex2D;
						Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(instanceTextureBuffer, instanceTextureBuffer);
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
						ImGui::SetTooltip(instanceTextureBuffer);
					}

					// drag & drop
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
						{
							std::string texID((const char*)payload->Data, payload->DataSize);
							Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(texID);
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
					std::vector<std::string> faces = cubemap->GetTempFileIds();
					for (size_t i = 0; i < 6; i++)
					{
						char InstanceCubemapBuffer[128];
						strncpy_s(InstanceCubemapBuffer, faces[i].c_str(), sizeof(InstanceCubemapBuffer));
						InstanceCubemapBuffer[sizeof(InstanceCubemapBuffer) - 1] = '\0';

						ImGui::SetCursorPosX(pLabelWidth);
						ImGui::SetNextItemWidth(pInputWidth);
						
						ImGui::PushID(static_cast<int>(i));
						if (ImGui::InputText(("##FaceInst" + std::to_string(i)).c_str(), InstanceCubemapBuffer, sizeof(InstanceCubemapBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
						{
							faces[i] = InstanceCubemapBuffer;
							cubemap->SetTempFileIds(faces);
						}

						if (ImGui::IsItemHovered())
						{
							ImGui::SetTooltip(InstanceCubemapBuffer);
						}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
							{
								std::string texID((const char*)payload->Data, payload->DataSize);
								faces[i] = texID;
								cubemap->SetTempFileIds(faces);
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::PopID();
					}

					ImGui::SetCursorPosX(pLabelWidth);
					if (ImGui::Button(("Create Cubemap##Inst" + name).c_str(), ImVec2(pInputWidth, 0)))
					{
						std::string nameCm;
						for (auto face : faces)
						{
							nameCm += face;
						}
						Zephyrus::Assets::ICubeMapTexture* newCubemap = Zephyrus::Assets::AssetsManager::GetInstance().LoadCubemap(faces, nameCm);
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
		ImGui::PushID(("floatInst" + pIndex).c_str());
		if (!floatOverrides.empty() && ImGui::TreeNode("Float##Inst"))
		{
			for (auto& [name, value] : floatOverrides)
			{
				std::string floatLabel = "FloatInst" + name + pIndex;
				ImGui::PushID(floatLabel.c_str());
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));
				ImGui::Text("%s", name.c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				float v = value;
				std::string valueLabel = "##ValueInst" + name + pIndex;
				ImGui::InputFloat(valueLabel.c_str(), &v, 0.0f, 0.0f, "%.2f");
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
		ImGui::PushID(("intInst" + pIndex).c_str());
		if (!intOverrides.empty() && ImGui::TreeNode("Int##Inst"))
		{
			for (auto& [name, value] : intOverrides)
			{
				std::string intLabel = "IntInst" + name + pIndex;
				ImGui::PushID(intLabel.c_str());
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));
				ImGui::Text("%s", name.c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				int v = value;
				std::string valueLabel = "##ValueInst" + name + pIndex;
				ImGui::InputInt(valueLabel.c_str(), &v, 0, 0);
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
		ImGui::PushID(("vec2Inst" + pIndex).c_str());
		if (!vec2Overrides.empty() && ImGui::TreeNode("Vector 2D##Inst"))
		{
			for (auto& [name, value] : vec2Overrides)
			{
				std::string vec2Label = "Vec2Inst" + name + pIndex;
				ImGui::PushID(vec2Label.c_str());
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));
				ImGui::Text("%s", name.c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				float v[2] = { value.x, value.y };
				std::string valueLabel = "##ValueInst" + name + pIndex;
				ImGui::InputFloat2(valueLabel.c_str(), v, "%.3f");
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
		ImGui::PushID(("vec3Inst" + pIndex).c_str());
		if (!vec3Overrides.empty() && ImGui::TreeNode("Vector 3D##Inst"))
		{
			for (auto& [name, value] : vec3Overrides)
			{
				std::string vec3Label = "Vec3Inst" + name + pIndex;
				ImGui::PushID(vec3Label.c_str());
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));
				ImGui::Text("%s", name.c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth);

				float v[3] = { value.x, value.y, value.z };
				std::string valueLabel = "##ValueInst" + name + pIndex;
				ImGui::InputFloat3(valueLabel.c_str(), v, "%.3f");
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
		ImGui::PushID(("vec4Inst" + pIndex).c_str());
		if (!vec4Overrides.empty() && ImGui::TreeNode("Vector 4D##Inst"))
		{
			for (auto& [name, value] : vec4Overrides)
			{
				std::string vec4Label = "Vec2Inst" + name + pIndex;
				ImGui::PushID(vec4Label.c_str());
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));
				ImGui::Text("%s", name.c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine(pLabelWidth);
				ImGui::SetNextItemWidth(pInputWidth / 4 * 3);

				float v[4] = { value.x, value.y, value.z, value.w };
				static float newVec4[4];

				std::string valueLabel = "##ValueInst" + name + pIndex;
				ImGui::InputFloat4(valueLabel.c_str(), v, "%.2f");

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

				ImGui::SameLine();

				std::string colorLabel = "ColorInst" +vec4Label + "color";
				ImGui::PopID();
				ImGui::PushID(colorLabel.c_str());
				if (ImGui::ColorButton("##colorInst", ImVec4(v[0], v[1], v[2], v[3]), 0, ImVec2(pInputWidth / 4, 0)))
				{
					ImGui::OpenPopup("ColorPopupOverride");
					newVec4[0] = value.x;
					newVec4[1] = value.y;
					newVec4[2] = value.z;
					newVec4[3] = value.w;
				}
				if (ImGui::BeginPopup("ColorPopupOverride", ImGuiWindowFlags_NoMove))
				{
					ImGui::ColorPicker4(colorLabel.c_str(), newVec4);
					if (ImGui::Button("Save Color##Inst"))
					{
						Vector4D oldVal = value;
						Vector4D newVal(newVec4[0], newVec4[1], newVec4[2], newVec4[3]);
						auto* evt = new SetGenericPropertyEvent<Vector4D>(
							nullptr,
							oldVal,
							newVal,
							[instance, name](const Vector4D& val) { instance->SetVector4D(name, val); }
						);
						EventSystem::DoEvent(evt);
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
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
	auto prop = MakeUndoableProperty<std::vector<std::pair<std::string, float>>>(property, mActiveComponent);
	auto* floats = static_cast<std::vector<std::pair<std::string, float>>*>(prop.getter());
	if (!floats)
	{
		return false;
	}
	ImGuiTreeNodeFlags flags = floats->empty() ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Float Parameters", flags))
	{
		for (size_t i = 0; i < floats->size(); i++)
		{
			std::string propertyName = floats->at(i).first;
			float floatingNumber = floats->at(i).second;
			std::string name = "Float " + std::to_string(i);

			ImGui::PushID(name.c_str());
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));

			char propertyNameBuffer[256];
			strncpy_s(propertyNameBuffer, propertyName.c_str(), sizeof(propertyNameBuffer));
			propertyNameBuffer[sizeof(propertyNameBuffer) - 1] = '\0';
			
			ImGui::SetNextItemWidth(pLabelWidth * 1.8f);
			if (ImGui::InputText(("##Prop" + name).c_str(), propertyNameBuffer, sizeof(propertyNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				auto newVec = *floats;
				auto it = std::find_if(newVec.begin(), newVec.end(),
				[&propertyNameBuffer](const auto& pair) { return pair.first == propertyNameBuffer; });
				if (it == newVec.end())
				{
					newVec[i] = std::pair<std::string, float>(propertyNameBuffer, floatingNumber);
					prop.Set(&newVec);	
				}
			}
			
			ImGui::PopStyleColor();
			ImGui::SameLine(pLabelWidth * 2);
			ImGui::SetNextItemWidth(pInputWidth - pLabelWidth * 1.5f);

			float tempFloat = floatingNumber;
			static float newVec4[4];

			ImGui::InputFloat("##Value", &tempFloat, 0, 0, "%.2f");

			if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				auto newVec = *floats;
				newVec[i] = std::pair<std::string, float>(propertyNameBuffer, tempFloat);
				prop.Set(&newVec);
			}

			ImGui::SameLine();

			if (!floats->empty())
			{
				ImGui::SameLine();
				if (ImGui::Button("Remove"))
				{
					auto removedVec = *floats;
					removedVec.erase(removedVec.begin() + static_cast<size_t>(i));
					prop.Set(&removedVec);
		
					ImGui::PopID();
					break;
				}
			}
			if (i < floats->size() - 1)
			{
				ImGui::Separator();
			}
			ImGui::PopID();
		}
		if (ImGui::Button("+ Add Float"))
		{
			mNameProp = "uFloat" + std::to_string(floats->size());
			auto newVec = *floats;
			newVec.emplace_back(mNameProp, 1);
			prop.Set(&newVec);	
		}
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyArrayInt(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::vector<std::pair<std::string, int>>>(property, mActiveComponent);
	auto* ints = static_cast<std::vector<std::pair<std::string, int>>*>(prop.getter());
	if (!ints)
	{
		return false;
	}
	ImGuiTreeNodeFlags flags = ints->empty() ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Int Parameters", flags))
	{
		for (size_t i = 0; i < ints->size(); i++)
		{
			std::string propertyName = ints->at(i).first;
			int integer = ints->at(i).second;
			std::string name = "Int " + std::to_string(i);

			ImGui::PushID(name.c_str());
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));

			char propertyNameBuffer[256];
			strncpy_s(propertyNameBuffer, propertyName.c_str(), sizeof(propertyNameBuffer));
			propertyNameBuffer[sizeof(propertyNameBuffer) - 1] = '\0';
			
			ImGui::SetNextItemWidth(pLabelWidth * 1.8f);
			if (ImGui::InputText(("##Prop" + name).c_str(), propertyNameBuffer, sizeof(propertyNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				auto newVec = *ints;
				auto it = std::find_if(newVec.begin(), newVec.end(),
				[&propertyNameBuffer](const auto& pair) { return pair.first == propertyNameBuffer; });
				if (it == newVec.end())
				{
					newVec[i] = std::pair<std::string, int>(propertyNameBuffer, integer);
					prop.Set(&newVec);	
				}
			}
			
			ImGui::PopStyleColor();
			ImGui::SameLine(pLabelWidth * 2);
			ImGui::SetNextItemWidth(pInputWidth - pLabelWidth * 1.5f);

			int tempInt = integer;
			static float newVec4[4];

			ImGui::InputInt("##Value", &tempInt, 0);

			if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				auto newVec = *ints;
				newVec[i] = std::pair<std::string, int>(propertyNameBuffer, tempInt);
				prop.Set(&newVec);
			}

			ImGui::SameLine();

			if (!ints->empty())
			{
				ImGui::SameLine();
				if (ImGui::Button("Remove"))
				{
					auto removedVec = *ints;
					removedVec.erase(removedVec.begin() + static_cast<size_t>(i));
					prop.Set(&removedVec);
		
					ImGui::PopID();
					break;
				}
			}
			if (i < ints->size() - 1)
			{
				ImGui::Separator();
			}
			ImGui::PopID();
		}
		if (ImGui::Button("+ Add Int"))
		{
			mNameProp = "uInt" + std::to_string(ints->size());
			auto newVec = *ints;
			newVec.emplace_back(mNameProp, 1);
			prop.Set(&newVec);	
		}
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyArrayVector2D(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::vector<std::pair<std::string, Vector2D>>>(property, mActiveComponent);
	auto* vectors = static_cast<std::vector<std::pair<std::string, Vector2D>>*>(prop.getter());
	if (!vectors)
	{
		return false;
	}
	ImGuiTreeNodeFlags flags = vectors->empty() ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Vector2D Parameters", flags))
	{
		for (size_t i = 0; i < vectors->size(); i++)
		{
			std::string propertyName = vectors->at(i).first;
			Vector2D vec3 = vectors->at(i).second;
			std::string name = "Vector2D " + std::to_string(i);

			ImGui::PushID(name.c_str());
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));

			char propertyNameBuffer[256];
			strncpy_s(propertyNameBuffer, propertyName.c_str(), sizeof(propertyNameBuffer));
			propertyNameBuffer[sizeof(propertyNameBuffer) - 1] = '\0';
			
			ImGui::SetNextItemWidth(pLabelWidth * 1.8f);
			if (ImGui::InputText(("##Prop" + name).c_str(), propertyNameBuffer, sizeof(propertyNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				auto newVec = *vectors;
				auto it = std::find_if(newVec.begin(), newVec.end(),
				[&propertyNameBuffer](const auto& pair) { return pair.first == propertyNameBuffer; });
				if (it == newVec.end())
				{
					newVec[i] = std::pair<std::string, Vector2D>(propertyNameBuffer, vec3);
					prop.Set(&newVec);	
				}
			}
			
			ImGui::PopStyleColor();
			ImGui::SameLine(pLabelWidth * 2);
			ImGui::SetNextItemWidth(pInputWidth - pLabelWidth * 1.5f);

			float v[2] = { vec3.x, vec3.y};

			ImGui::InputFloat2("##Value", v, "%.2f");

			if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				auto newVec = *vectors;
				Vector2D newVector3D(v[0], v[1]);
				newVec[i] = std::pair<std::string, Vector2D>(propertyNameBuffer, newVector3D);
				prop.Set(&newVec);
			}

			ImGui::SameLine();

			if (!vectors->empty())
			{
				ImGui::SameLine();
				if (ImGui::Button("Remove"))
				{
					auto removedVec = *vectors;
					removedVec.erase(removedVec.begin() + static_cast<size_t>(i));
					prop.Set(&removedVec);
		
					ImGui::PopID();
					break;
				}
			}
			if (i < vectors->size() - 1)
			{
				ImGui::Separator();
			}
			ImGui::PopID();
		}
		if (ImGui::Button("+ Add Vec2"))
		{
			mNameProp = "uVector2D" + std::to_string(vectors->size());
			auto newVec = *vectors;
			newVec.emplace_back(mNameProp, Vector2D(1.0f, 1.0f));
			prop.Set(&newVec);	
		}
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyArrayVector3D(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::vector<std::pair<std::string, Vector3D>>>(property, mActiveComponent);
	auto* vectors = static_cast<std::vector<std::pair<std::string, Vector3D>>*>(prop.getter());
	if (!vectors)
	{
		return false;
	}
	ImGuiTreeNodeFlags flags = vectors->empty() ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Vector3D Parameters", flags))
	{
		for (size_t i = 0; i < vectors->size(); i++)
		{
			std::string propertyName = vectors->at(i).first;
			Vector3D vec3 = vectors->at(i).second;
			std::string name = "Vector3D " + std::to_string(i);

			ImGui::PushID(name.c_str());
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));

			char propertyNameBuffer[256];
			strncpy_s(propertyNameBuffer, propertyName.c_str(), sizeof(propertyNameBuffer));
			propertyNameBuffer[sizeof(propertyNameBuffer) - 1] = '\0';
			
			ImGui::SetNextItemWidth(pLabelWidth * 1.8f);
			if (ImGui::InputText(("##Prop" + name).c_str(), propertyNameBuffer, sizeof(propertyNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				auto newVec = *vectors;
				auto it = std::find_if(newVec.begin(), newVec.end(),
				[&propertyNameBuffer](const auto& pair) { return pair.first == propertyNameBuffer; });
				if (it == newVec.end())
				{
					newVec[i] = std::pair<std::string, Vector3D>(propertyNameBuffer, vec3);
					prop.Set(&newVec);	
				}
			}
			
			ImGui::PopStyleColor();
			ImGui::SameLine(pLabelWidth * 2);
			ImGui::SetNextItemWidth(pInputWidth - pLabelWidth * 1.5f);

			float v[3] = { vec3.x, vec3.y, vec3.z};

			ImGui::InputFloat3("##Value", v, "%.2f");

			if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				auto newVec = *vectors;
				Vector3D newVector3D(v[0], v[1], v[2]);
				newVec[i] = std::pair<std::string, Vector3D>(propertyNameBuffer, newVector3D);
				prop.Set(&newVec);
			}

			ImGui::SameLine();

			if (!vectors->empty())
			{
				ImGui::SameLine();
				if (ImGui::Button("Remove"))
				{
					auto removedVec = *vectors;
					removedVec.erase(removedVec.begin() + static_cast<size_t>(i));
					prop.Set(&removedVec);
		
					ImGui::PopID();
					break;
				}
			}
			if (i < vectors->size() - 1)
			{
				ImGui::Separator();
			}
			ImGui::PopID();
		}
		if (ImGui::Button("+ Add Vec3"))
		{
			mNameProp = "uVector3D" + std::to_string(vectors->size());
			auto newVec = *vectors;
			newVec.emplace_back(mNameProp, Vector3D(1.0f, 1.0f, 1.0f));
			prop.Set(&newVec);	
		}
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyArrayVector4D(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::vector<std::pair<std::string, Vector4D>>>(property, mActiveComponent);
	auto* vectors = static_cast<std::vector<std::pair<std::string, Vector4D>>*>(prop.getter());
	if (!vectors)
	{
		return false;
	}
	ImGuiTreeNodeFlags flags = vectors->empty() ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Vector4D Parameters", flags))
	{
		for (size_t i = 0; i < vectors->size(); i++)
		{
			std::string propertyName = vectors->at(i).first;
			Vector4D vec4 = vectors->at(i).second;
			std::string name = "Vector4D " + std::to_string(i);

			ImGui::PushID(name.c_str());
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));

			char propertyNameBuffer[256];
			strncpy_s(propertyNameBuffer, propertyName.c_str(), sizeof(propertyNameBuffer));
			propertyNameBuffer[sizeof(propertyNameBuffer) - 1] = '\0';
			
			ImGui::SetNextItemWidth(pLabelWidth * 1.8f);
			if (ImGui::InputText(("##Prop" + name).c_str(), propertyNameBuffer, sizeof(propertyNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				auto newVec = *vectors;
				auto it = std::find_if(newVec.begin(), newVec.end(),
				[&propertyNameBuffer](const auto& pair) { return pair.first == propertyNameBuffer; });
				if (it == newVec.end())
				{
					newVec[i] = std::pair<std::string, Vector4D>(propertyNameBuffer, vec4);
					prop.Set(&newVec);	
				}
			}
			
			ImGui::PopStyleColor();
			ImGui::SameLine(pLabelWidth * 2);
			ImGui::SetNextItemWidth((pInputWidth - pLabelWidth * 1.5f) * 0.8f);

			float v[4] = { vec4.x, vec4.y, vec4.z, vec4.w };
			static float newVec4[4];

			ImGui::InputFloat4("##Value", v, "%.2f");

			if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				auto newVec = *vectors;
				Vector4D newVector4D(v[0], v[1], v[2], v[3]);
				newVec[i] = std::pair<std::string, Vector4D>(propertyNameBuffer, newVector4D);
				prop.Set(&newVec);
			}

			ImGui::SameLine();

			std::string colorLabel = name + "color";
			ImGui::PopID();
			ImGui::PushID(colorLabel.c_str());
			if (ImGui::ColorButton("##color", ImVec4(v[0], v[1], v[2], v[3]), 0, ImVec2((pInputWidth - pLabelWidth * 1.5f) * 0.2f - 10, 0)))
			{
				ImGui::OpenPopup("ColorPopupOverride");
				newVec4[0] = vec4.x;
				newVec4[1] = vec4.y;
				newVec4[2] = vec4.z;
				newVec4[3] = vec4.w;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}
			if (ImGui::BeginPopup("ColorPopupOverride", ImGuiWindowFlags_NoMove))
			{
				ImGui::ColorPicker4(colorLabel.c_str(), newVec4);
				if (ImGui::Button("Save Color"))
				{
					auto newVec = *vectors;
					Vector4D newVector4D(newVec4[0], newVec4[1], newVec4[2], newVec4[3]);
					newVec[i] = std::pair<std::string, Vector4D>(propertyNameBuffer, newVector4D);
					prop.Set(&newVec);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (!vectors->empty())
			{
				ImGui::SameLine();
				if (ImGui::Button("Remove"))
				{
					auto removedVec = *vectors;
					removedVec.erase(removedVec.begin() + static_cast<size_t>(i));
					prop.Set(&removedVec);
		
					ImGui::PopID();
					break;
				}
			}
			if (i < vectors->size() - 1)
			{
				ImGui::Separator();
			}
			ImGui::PopID();
		}
		if (ImGui::Button("+ Add Vec4"))
		{
			mNameProp = "uVector4D" + std::to_string(vectors->size());
			auto newVec = *vectors;
			newVec.emplace_back(mNameProp, Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
			prop.Set(&newVec);	
		}
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyArrayTextureBase(const std::string& pIndex, const PropertyDescriptor& property, const float& pLabelWidth,
	const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<std::vector<std::pair<std::string, Zephyrus::Assets::ITextureBase*>>>(property, mActiveComponent);
	auto* textures = static_cast<std::vector<std::pair<std::string, Zephyrus::Assets::ITextureBase*>>*>(prop.getter());
	if (!textures)
	{
		return false;
	}
	ImGuiTreeNodeFlags flags = textures->empty() ? ImGuiTreeNodeFlags_None : ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Textures Parameters", flags))
	{
		for (size_t i = 0; i < textures->size(); i++)
		{
			std::string propertyName = textures->at(i).first;
			Zephyrus::Assets::ITextureBase* tex = textures->at(i).second;
			std::string name = "Texture " + std::to_string(i);
		
			ImGui::PushID(name.c_str());
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.0f, 1.0f));

			char propertyNameBuffer[256];
			strncpy_s(propertyNameBuffer, propertyName.c_str(), sizeof(propertyNameBuffer));
			propertyNameBuffer[sizeof(propertyNameBuffer) - 1] = '\0';
			
			ImGui::SetNextItemWidth(pLabelWidth * 1.8f);
			if (ImGui::InputText(("##Prop" + name).c_str(), propertyNameBuffer, sizeof(propertyNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				auto newVec = *textures;
				auto it = std::find_if(newVec.begin(), newVec.end(),
				[&propertyNameBuffer](const auto& pair) { return pair.first == propertyNameBuffer; });
				if (it == newVec.end())
				{
					newVec[i] = std::pair<std::string, Zephyrus::Assets::ITextureBase*>(propertyNameBuffer, tex);
					prop.Set(&newVec);	
				}
			}
			
			ImGui::PopStyleColor();
			ImGui::SameLine(pLabelWidth * 2);
			ImGui::SetNextItemWidth(pInputWidth - pLabelWidth * 1.5f);

			if (auto* tex2D = dynamic_cast<Zephyrus::Assets::ITexture2D*>(tex))
			{
				char buffer[256];
				strncpy_s(buffer, tex2D->GetTextureFileId().c_str(), sizeof(buffer));
				buffer[sizeof(buffer) - 1] = '\0';

				if (ImGui::InputText(("##" + name).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				{
					Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(buffer, buffer);
					if (newTex)
					{
						auto newVec = *textures;
						newVec[i] = std::pair<std::string, Zephyrus::Assets::ITextureBase*>(propertyNameBuffer, newTex);
						prop.Set(&newVec);
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
						Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(texID);
						if (newTex)
						{
							auto newVec = *textures;
							newVec[i] = std::pair<std::string, Zephyrus::Assets::ITextureBase*>(propertyNameBuffer, newTex);
							prop.Set(&newVec);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
			else if (auto* cubemap = dynamic_cast<Zephyrus::Assets::ICubeMapTexture*>(tex))
			{
				std::vector<std::string> faces = cubemap->GetTempFileIds();
				for (size_t j = 0; j < 6; j++)
				{
					char buffer[128];
					strncpy_s(buffer, faces[j].c_str(), sizeof(buffer));
					buffer[sizeof(buffer) - 1] = '\0';

					ImGui::SetCursorPosX(pLabelWidth * 2);
					ImGui::SetNextItemWidth(pInputWidth - pLabelWidth * 1.5f);
					
					ImGui::PushID(static_cast<int>(j));
					if (ImGui::InputText(("##Face" + std::to_string(j)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					{
						faces[i] = buffer;
						cubemap->SetTempFileIds(faces);
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
							faces[j] = texID;
							cubemap->SetTempFileIds(faces);
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::PopID();
				}

				ImGui::SetCursorPosX(pLabelWidth * 2.0f);
				if (ImGui::Button(("Create Cubemap##" + name).c_str(), ImVec2(pInputWidth - pLabelWidth * 1.5f , 0)))
				{
					std::string nameCm;
					for (auto face : faces)
					{
						nameCm += face;
					}
					Zephyrus::Assets::ICubeMapTexture* newCubemap = Zephyrus::Assets::AssetsManager::GetInstance().LoadCubemap(faces, nameCm);
					if (newCubemap)
					{
						auto newVec = *textures;
						newVec[i] = std::pair<std::string, Zephyrus::Assets::ITextureBase*>(propertyNameBuffer, newCubemap);
						prop.Set(&newVec);
					}
				}
			}

			if (!textures->empty())
			{
				ImGui::SameLine();
				if (ImGui::Button("Remove"))
				{
					auto newVec = *textures;
					newVec.erase(newVec.begin() + static_cast<size_t>(i));
					prop.Set(&newVec);
		
					ImGui::PopID();
					break;
				}
			}
			if (i < textures->size() - 1)
			{
				ImGui::Separator();
			}
			ImGui::PopID();
		}
		if (ImGui::Button("+ Add Texture"))
		{
			mNameProp = "uTexture" + std::to_string(textures->size());
			mTextureType = 0;
			ImGui::OpenPopup("NewTextureParameter");
		}
		if (ImGui::BeginPopup("NewTextureParameter", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("Add Texture");
			ImGui::Separator();
			
			ImGui::Text("Type : ");
			ImGui::SameLine(pLabelWidth);
			ImGui::Combo("##TextureType", &mTextureType, texureTypeStrings, 2);
			ImGui::Separator();
			
			ImGui::Text("Parameter name : ");
			ImGui::SameLine(pLabelWidth);
			
			char NewParameterNameBuffer[32];
			strncpy_s(NewParameterNameBuffer, mNameProp.c_str(), sizeof(NewParameterNameBuffer));
			NewParameterNameBuffer[sizeof(NewParameterNameBuffer) - 1] = '\0';

			auto newVec = *textures;
			auto it = std::find_if(newVec.begin(), newVec.end(),
			[&NewParameterNameBuffer](const auto& pair) { return pair.first == NewParameterNameBuffer; });
			if (it != newVec.end())
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			
			ImGui::InputText("##NewParameterName", NewParameterNameBuffer, sizeof(NewParameterNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);

			if (it != newVec.end())
			{
				ImGui::PopStyleColor();
			}
			
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(NewParameterNameBuffer);
			}
			
			mNameProp = NewParameterNameBuffer;
			
			Zephyrus::Assets::ITextureBase* tex = nullptr;
			if (mTextureType == 0)
			{
				tex = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(TEX_UV_MAPPER);
			}
			else
			{
				std::vector<std::string> mFilePaths;
				std::string cubemapName;
				for (int j = 0; j < 6; ++j)
				{
					mFilePaths.emplace_back(TEX_UV_MAPPER);
					cubemapName += TEX_UV_MAPPER;
				}
				tex = Zephyrus::Assets::AssetsManager::GetInstance().LoadCubemap(mFilePaths, cubemapName);
			}
			
			if (ImGui::Button("Accept##NewParameter"))
			{
				newVec = *textures;
				it = std::find_if(newVec.begin(), newVec.end(),
				[&NewParameterNameBuffer](const auto& pair) { return pair.first == NewParameterNameBuffer; });
				if (it != newVec.end())
				{
					mNameProp = NewParameterNameBuffer + std::to_string(textures->size());
				}
				newVec.emplace_back(mNameProp, tex);
				prop.Set(&newVec);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel##NewParameter"))
			{
				ImGui::CloseCurrentPopup();
			}
			
			ImGui::EndPopup();
		}
	}
	return true;
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
		strncpy_s(buffer, shader->GetShaderID().c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';
	}
	else
	{
		strncpy_s(buffer, std::string("None").c_str(), sizeof(buffer));
	}
	buffer[sizeof(buffer) - 1] = '\0';
	
	ImGui::Text(prop.name.c_str());

	ImGui::SameLine(pLabelWidth * 2);

	float buttonSize = 0.0f;
#ifdef _WIN32
	buttonSize = ImGui::CalcTextSize("Open").x + 10;
#endif
	
	ImGui::SetNextItemWidth(pInputWidth - pLabelWidth * 1.5f - buttonSize);
	std::string label = "##Shader" + std::string(buffer) + pIndex;
	
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		if (buffer[0] == '\0')
		{
			strncpy_s(buffer, std::string("None").c_str(), sizeof(buffer));
			prop.Set(nullptr);
			return true;
		}
		Zephyrus::Render::IShader* newShader = Zephyrus::Assets::AssetsManager::GetInstance().LoadShader(buffer, pType);
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
			Zephyrus::Render::IShader* droppedShader = Zephyrus::Assets::AssetsManager::GetInstance().LoadShader(shaderID, pType);
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
	
#ifdef _WIN32
	ImGui::SameLine();
	if (!shader)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button(("Open##" + pIndex).c_str(), ImVec2( buttonSize, 0)))
	{
			std::filesystem::path path = shader->GetShaderID();
			ShellExecuteA(nullptr, "open", path.make_preferred().string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	}
	if (!shader)
	{
		ImGui::EndDisabled();
	}
#endif
	
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
