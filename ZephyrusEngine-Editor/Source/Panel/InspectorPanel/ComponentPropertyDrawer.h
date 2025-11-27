#pragma once

#include "../Panel.h"
#include "../SceneHierarchyPanel.h"
#include "CameraComponent.h"

#include <unordered_map>
#include <functional>

namespace Zephyrus::Render
{
	enum class ShaderType;
}

using Zephyrus::ActorComponent::Actor;
using Zephyrus::ActorComponent::Component;
using Zephyrus::ActorComponent::CameraComponent;
using Zephyrus::ActorComponent::ActorState;

class Component;
struct PropertyDescriptor;

class ComponentPropertyDrawer
{
private:
	std::unordered_map<PropertyType, std::function<bool(const std::string&, const PropertyDescriptor&, float, float)>> mPropertySetters;
	Component* mActiveComponent = nullptr;
public:
	ComponentPropertyDrawer();
	~ComponentPropertyDrawer() = default;

	void DrawProperty(const std::string& pIndex, const PropertyDescriptor& pProperty, Zephyrus::ActorComponent::Component* activeComponent);

private:
	std::unordered_map<std::string, float> mEditingFloats;
	std::unordered_map<std::string, int> mEditingInts;
	
private:
	bool SetPropertyFloat(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyInt(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyBool(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyString(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyColor(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyVector4D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyVector3D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyVector2D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyQuaternion(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyTexture(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyFont(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyMesh(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyCubemap(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyPrefab(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyComponent(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayTexture2D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyMaterialInstance(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderVert(const std::string& pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderFrag(const std::string& pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderTesc(const std::string& pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderTese(const std::string& pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderGeom(const std::string& pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayFloat(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayInt(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayVector2D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayVector3D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayVector4D(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayTextureBase(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyTextureBase(const std::string& pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);

private:
	bool SetPropertyShader(const std::string& pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth, Zephyrus::Render::ShaderType pType);
};