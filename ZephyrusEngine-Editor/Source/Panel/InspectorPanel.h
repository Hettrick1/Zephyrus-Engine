#pragma once
#include "Panel.h"
#include "SceneHierarchyPanel.h"

class Component;
struct PropertyDescriptor;

class InspectorPanel : public Panel
{
private:
	SceneHierarchyPanel* mHierarchy = nullptr;
	Component* mActiveComponent = nullptr;
public:
	InspectorPanel(const std::string& pName);
	~InspectorPanel();
	void Draw() override;
	void DrawActorComponents(Actor* pActor);
	void DrawActorInfos(Actor* pActor);
	void DrawComponentInfos();
	void DrawProperty(const PropertyDescriptor& property);
	void DrawSplitterButton(float& h);
	void SetSceneHierarchy(SceneHierarchyPanel* pHierarchy);
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
