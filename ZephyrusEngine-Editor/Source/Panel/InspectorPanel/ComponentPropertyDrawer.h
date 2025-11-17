#pragma once

#include "../Panel.h"
#include "../SceneHierarchyPanel.h"
#include "CameraComponent.h"

#include <unordered_map>
#include <functional>

using Zephyrus::ActorComponent::Actor;
using Zephyrus::ActorComponent::Component;
using Zephyrus::ActorComponent::CameraComponent;
using Zephyrus::ActorComponent::ActorState;

class Component;
struct PropertyDescriptor;

class ComponentPropertyDrawer
{
private:
	std::unordered_map<PropertyType, std::function<bool(const PropertyDescriptor&, float, float)>> mPropertySetters;
	Component* mActiveComponent = nullptr;
public:
	ComponentPropertyDrawer();
	~ComponentPropertyDrawer() = default;

	void DrawProperty(const PropertyDescriptor& property, Zephyrus::ActorComponent::Component* activeComponent);

public:
	bool SetPropertyFloat(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyInt(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyBool(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyString(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyColor(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyVector4D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyVector3D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyVector2D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyQuaternion(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyTexture(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyFont(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyMesh(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyCubemap(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyPrefab(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyComponent(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayTexture2D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyMaterialInstance(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyShader(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayFloat(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayInt(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayVector2D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayVector3D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayVector4D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	bool SetPropertyArrayTextureBase(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
};