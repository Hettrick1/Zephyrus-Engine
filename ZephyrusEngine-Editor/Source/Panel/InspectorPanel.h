#pragma once
#include "Panel.h"
#include "SceneHierarchyPanel.h"
#include "CameraComponent.h"

class Component;
struct PropertyDescriptor;

class InspectorPanel : public Panel
{
private:
	SceneHierarchyPanel* mHierarchy = nullptr;
	Zephyrus::ActorComponent::Component* mActiveComponent = nullptr;
public:
	InspectorPanel(const std::string& pName);
	~InspectorPanel();
	void Draw() override;
	void DrawActorComponents(Zephyrus::ActorComponent::Actor* pActor);
	void DrawActorInfos(Zephyrus::ActorComponent::Actor* pActor);
	void DrawComponentInfos();
	void DrawProperty(const PropertyDescriptor& property);
	void DrawSplitterButton(float& h);
	void SetSceneHierarchy(SceneHierarchyPanel* pHierarchy);

	inline Zephyrus::ActorComponent::CameraComponent* GetCurrentCameraComponent() const
	{
		Zephyrus::ActorComponent::CameraComponent* cameraComp = dynamic_cast<Zephyrus::ActorComponent::CameraComponent*>(mActiveComponent);
		if (cameraComp)
		{
			return cameraComp;
		}
		return nullptr;
	}

public:
	void SetPropertyFloat(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyInt(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyBool(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
	void SetPropertyString(const PropertyDescriptor& property, const float& pLabelWidth, const float& pInputWidth);
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
};
