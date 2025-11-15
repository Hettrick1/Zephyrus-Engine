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
	std::unordered_map<PropertyType, std::function<void(const PropertyDescriptor&, float, float)>> mPropertySetters;
	Component* mActiveComponent = nullptr;
public:
	ComponentPropertyDrawer();
	~ComponentPropertyDrawer() = default;

	void DrawProperty(const PropertyDescriptor& property, Zephyrus::ActorComponent::Component* activeComponent);

public:
	void SetPropertyFloat(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyInt(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyBool(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyString(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyColor(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyVector4D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyVector3D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyVector2D(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyQuaternion(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyTexture(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyFont(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyMesh(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyCubemap(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyPrefab(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyComponent(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyVectorTexture(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyMaterialInstance(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
};