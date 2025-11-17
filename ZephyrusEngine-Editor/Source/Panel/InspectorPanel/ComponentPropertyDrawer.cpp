#include "ComponentPropertyDrawer.h"
#include "Actor.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/Event/RenameActorEvent.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "EditorApplication/EventSystem/Event/SetTransformEvent.h"
#include "Assets.h"
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
	mPropertySetters[PropertyType::Float] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyFloat(p, lw, iw); };
	mPropertySetters[PropertyType::Int] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyInt(p, lw, iw); };
	mPropertySetters[PropertyType::Bool] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyBool(p, lw, iw); };
	mPropertySetters[PropertyType::String] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyString(p, lw, iw); };
	mPropertySetters[PropertyType::Vec2] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyVector2D(p, lw, iw); };
	mPropertySetters[PropertyType::Vec3] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyVector3D(p, lw, iw); };
	mPropertySetters[PropertyType::Vec4] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyVector4D(p, lw, iw); };
	mPropertySetters[PropertyType::Color] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyColor(p, lw, iw); };
	mPropertySetters[PropertyType::Quaternion] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyQuaternion(p, lw, iw); };
	mPropertySetters[PropertyType::Texture] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyTexture(p, lw, iw); };
	mPropertySetters[PropertyType::Font] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyFont(p, lw, iw); };
	mPropertySetters[PropertyType::Mesh] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyMesh(p, lw, iw); };
	mPropertySetters[PropertyType::VectorTexture] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyVectorTexture(p, lw, iw); };
	mPropertySetters[PropertyType::Prefab] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyPrefab(p, lw, iw); };
	mPropertySetters[PropertyType::CubeMap] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyCubemap(p, lw, iw); };
	mPropertySetters[PropertyType::Component] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyComponent(p, lw, iw); };
	mPropertySetters[PropertyType::MaterialInstance] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyMaterialInstance(p, lw, iw); };
	mPropertySetters[PropertyType::Shader] = [this](const PropertyDescriptor& p, float lw, float iw) { return SetPropertyShader(p, lw, iw); };
	
}

void ComponentPropertyDrawer::DrawProperty(const PropertyDescriptor& property, Zephyrus::ActorComponent::Component* activeComponent)
{
	mActiveComponent = activeComponent;
	ImGui::AlignTextToFramePadding();
	float labelWidth = 125;
	float inputWidth = ImGui::GetContentRegionAvail().x - labelWidth;

	auto it = mPropertySetters.find(property.type);
	if (it != mPropertySetters.end())
	{
		if (it->second(property, labelWidth, inputWidth))
		{
			ImGui::Separator();
		}
	}
}

bool ComponentPropertyDrawer::SetPropertyFloat(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<float>(pProperty, mActiveComponent);
	float fVar = *static_cast<float*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name;
	ImGui::InputFloat(label.c_str(), &fVar);
	if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
	{
		prop.setter(&fVar);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyInt(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<int>(pProperty, mActiveComponent);
	int iVar = *static_cast<int*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name;
	ImGui::InputInt(label.c_str(), &iVar);
	if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter))
	{
		prop.setter(&iVar);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyBool(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
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
	return true;
}

bool ComponentPropertyDrawer::SetPropertyString(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
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
	return true;
}

bool ComponentPropertyDrawer::SetPropertyColor(const PropertyDescriptor& pProperty, const float& pLabelWidth,
	const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Vector4D>(pProperty, mActiveComponent);
	Vector4D colorVar = *static_cast<Vector4D*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name;
	
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
			prop.setter(&newColorVar);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyVector4D(const PropertyDescriptor& pProperty, const float& pLabelWidth,
	const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Vector4D>(pProperty, mActiveComponent);
	Vector4D vec4Var = *static_cast<Vector4D*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	std::string label = "##" + prop.name;
	float vec4[4] = { vec4Var.x, vec4Var.y, vec4Var.z, vec4Var.w };
	if (ImGui::InputFloat4(label.c_str(), vec4, "%.3f", ImGuiInputTextFlags_AutoSelectAll))
	{
		vec4Var = Vector4D(vec4[0], vec4[1], vec4[2], vec4[3]);
		prop.setter(&vec4Var);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyVector3D(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
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
	return true;
}

bool ComponentPropertyDrawer::SetPropertyVector2D(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
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
	return true;
}

bool ComponentPropertyDrawer::SetPropertyQuaternion(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	// TODO
	return false;
}

bool ComponentPropertyDrawer::SetPropertyTexture(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	Property prop;
	prop = MakeUndoableProperty<Zephyrus::Assets::ITexture2D*>(pProperty, mActiveComponent);
	Zephyrus::Assets::ITexture2D* tex = static_cast<Zephyrus::Assets::ITexture2D*>(prop.getter());
	if (!tex)
	{
		return false;
	}
	char buffer[255];
	strncpy(buffer, tex->GetFilePath().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Texture : ");

	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	if (ImGui::InputText(("##Texture" + std::string(buffer)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		Zephyrus::Assets::ITexture2D* newTex = Zephyrus::Assets::AssetsManager::LoadTexture(buffer, buffer);
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
			Zephyrus::Assets::ITexture2D* droppedTex = Zephyrus::Assets::AssetsManager::LoadTexture(textureID, textureID);
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
	return true;
}

bool ComponentPropertyDrawer::SetPropertyFont(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	// TODO
	return false;
}

bool ComponentPropertyDrawer::SetPropertyMesh(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Zephyrus::Assets::IMesh*>(pProperty, mActiveComponent);
	Zephyrus::Assets::IMesh* mesh = static_cast<Zephyrus::Assets::IMesh*>(prop.getter());
	if (!mesh)
	{
		return false;
	}
	char buffer[255];
	strncpy(buffer, mesh->GetFilePath().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Mesh : ");

	ImGui::SameLine(pLabelWidth);

	ImGui::SetNextItemWidth(pInputWidth);
	if (ImGui::InputText(("##Mesh" + std::string(buffer)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		Zephyrus::Assets::IMesh* newMesh = Zephyrus::Assets::AssetsManager::LoadMesh(buffer, buffer);
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
			Zephyrus::Assets::IMesh* droppedMesh = Zephyrus::Assets::AssetsManager::LoadMesh(meshID, meshID);
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
	return true;
}

bool ComponentPropertyDrawer::SetPropertyCubemap(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
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
		Zephyrus::Assets::ICubeMapTexture* newCubemap = Zephyrus::Assets::AssetsManager::LoadCubemap(newFaces, newFaces[0]);
		if (!newCubemap)
		{
			ZP_CORE_ERROR("Cubemap creation failed!");
			return false;
		}
		cubemap->SetTempFilePath(cubemap->GetFaceFilePath());
		prop.setter(newCubemap);
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyPrefab(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
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
			std::string prefabID((const char*)payload->Data, payload->DataSize);
			prop.setter(&prefabID);
		}
		ImGui::EndDragDropTarget();
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyComponent(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
{
	// TODO : test and fix
	
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
	return true;
}

bool ComponentPropertyDrawer::SetPropertyVectorTexture(const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth)
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
				strncpy(buffer, tex->GetFilePath().c_str(), sizeof(buffer));
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
					prop.setter(&newVec);
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
	return true;
}

bool ComponentPropertyDrawer::SetPropertyMaterialInstance(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth)
{
	auto prop = MakeUndoableProperty<Zephyrus::Material::MaterialInstance>(property, mActiveComponent);
	auto* instance = static_cast<Zephyrus::Material::MaterialInstance*>(prop.getter());
	ImGui::Text(prop.name.c_str());
	char buffer[255];
	strncpy(buffer, instance->GetBaseMaterial()->GetFilePath().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text("Base Material : ");
	ImGui::SameLine(pLabelWidth);
	ImGui::SetNextItemWidth(pInputWidth);
	if (ImGui::InputText(("##String" + std::string(buffer)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		auto newMaterial = Zephyrus::Assets::AssetsManager::LoadMaterial(buffer, buffer);
		if (newMaterial == instance->GetBaseMaterial())
		{
			return true;
		}

		auto materialInstance = Zephyrus::Material::MaterialInstance();
		materialInstance.SetMaterial(newMaterial);
		prop.setter(&materialInstance);
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(buffer);
	}

	if (ImGui::TreeNodeEx("Material Properties", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// --- TEXTURES OVERRIDES ---

		auto texOverrides = instance->GetTextureOverrides();
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
					strncpy(buffer, tex2D->GetFilePath().c_str(), sizeof(buffer));
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
						strncpy(buffer, faces[i].c_str(), sizeof(buffer));
						buffer[sizeof(buffer) - 1] = '\0';

						ImGui::PushID((int)i);
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

		// --- FLOAT OVERRIDES ---
		auto floatOverrides = instance->GetFloatOverrides();
		if (!floatOverrides.empty() && ImGui::TreeNode("Float"))
		{
			for (auto& [name, value] : floatOverrides)
			{
				ImGui::PushID(name.c_str());
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

		// --- INT OVERRIDES ---
		auto intOverrides = instance->GetIntOverrides();
		if (!intOverrides.empty() && ImGui::TreeNode("Int"))
		{
			for (auto& [name, value] : intOverrides)
			{
				ImGui::PushID(name.c_str());
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

		// --- VECTOR2 OVERRIDES ---
		auto vec2Overrides = instance->GetVec2Overrides();
		if (!vec2Overrides.empty() && ImGui::TreeNode("Vector 2D"))
		{
			for (auto& [name, value] : vec2Overrides)
			{
				ImGui::PushID(name.c_str());
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

		// --- VECTOR3 OVERRIDES ---
		auto vec3Overrides = instance->GetVec3Overrides();
		if (!vec3Overrides.empty() && ImGui::TreeNode("Vector 3D"))
		{
			for (auto& [name, value] : vec3Overrides)
			{
				ImGui::PushID(name.c_str());
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

		// --- VECTOR4 OVERRIDES ---
		auto vec4Overrides = instance->GetVec4Overrides();
		if (!vec4Overrides.empty() && ImGui::TreeNode("Vector 4D"))
		{
			for (auto& [name, value] : vec4Overrides)
			{
				ImGui::PushID(name.c_str());
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

		ImGui::TreePop();
	}
	return true;
}

bool ComponentPropertyDrawer::SetPropertyShader(const PropertyDescriptor& pProperty, const float& pLabelWidth,
	const float& pInputWidth)
{
	Property prop;
	prop = MakeUndoableProperty<Zephyrus::Render::IShader*>(pProperty, mActiveComponent);
	Zephyrus::Render::IShader* shader = static_cast<Zephyrus::Render::IShader*>(prop.getter());
	if (!shader)
	{
		return false;
	}
	char buffer[255];
	strncpy(buffer, shader->GetFilePath().c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';

	ImGui::Text(prop.name.c_str());

	ImGui::SameLine(pLabelWidth * 2);

	ImGui::SetNextItemWidth(pInputWidth);
	if (ImGui::InputText(("##Texture" + std::string(buffer)).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
	{
		// TODO : For now I use the same type as the previous shader. Maybe use the extension to know the type
		Zephyrus::Render::IShader* newShader = Zephyrus::Assets::AssetsManager::LoadShader(buffer, shader->GetType() ,buffer);
		if (newShader)
		{
			prop.setter(newShader);
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
			Zephyrus::Render::IShader* droppedShader = Zephyrus::Assets::AssetsManager::LoadShader(shaderID, shader->GetType(), shaderID);
			if (droppedShader)
			{
				prop.setter(droppedShader);
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
