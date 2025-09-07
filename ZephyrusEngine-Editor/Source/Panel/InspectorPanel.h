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
	void DrawActorInfos(Actor* pActor);
	void DrawSplitterButton(float& h);
	void SetSceneHierarchy(SceneHierarchyPanel* pHierarchy);
};
