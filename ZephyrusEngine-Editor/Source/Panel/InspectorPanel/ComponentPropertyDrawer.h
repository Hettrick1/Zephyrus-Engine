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
	std::unordered_map<PropertyType, std::function<bool(unsigned int, const PropertyDescriptor&, float, float)>> mPropertySetters;
	Component* mActiveComponent = nullptr;
public:
	ComponentPropertyDrawer();
	~ComponentPropertyDrawer() = default;

	void DrawProperty(unsigned int pIndex, const PropertyDescriptor& pProperty, Zephyrus::ActorComponent::Component* activeComponent);

private:
	std::unordered_map<std::string, float> mEditingFloats;
	std::unordered_map<std::string, int> mEditingInts;
	
private:
	bool SetPropertyFloat(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyInt(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyBool(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyString(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyColor(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyVector4D(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyVector3D(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyVector2D(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyQuaternion(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyTexture(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyFont(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyMesh(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyCubemap(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyPrefab(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyComponent(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayTexture2D(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyMaterialInstance(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderVert(unsigned int pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderFrag(unsigned int pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderTesc(unsigned int pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderTese(unsigned int pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShaderGeom(unsigned int pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayFloat(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayInt(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayVector2D(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayVector3D(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayVector4D(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayTextureBase(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyTextureBase(unsigned int pIndex, const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth);

private:
	bool SetPropertyShader(unsigned int pIndex,const PropertyDescriptor& pProperty, const float& pLabelWidth, const float& pInputWidth, Zephyrus::Render::ShaderType pType);
};