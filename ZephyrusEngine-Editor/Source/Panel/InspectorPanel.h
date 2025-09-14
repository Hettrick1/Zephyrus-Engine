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
};
