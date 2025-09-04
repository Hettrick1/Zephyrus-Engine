#pragma once
#include "Panel.h"
#include "SceneHierarchyPanel.h"

class InspectorPanel : public Panel
{
private:
	SceneHierarchyPanel* mHierarchy = nullptr;
public:
	InspectorPanel(const std::string& pName);
	~InspectorPanel();
	void Draw() override;
	void DrawActorComponents(Actor* pActor);
	void SetSceneHierarchy(SceneHierarchyPanel* pHierarchy);
};
