#pragma once
#include "Panel.h"

class Actor;

class SceneHierarchyPanel : public Panel
{
private:
	Actor* mSelectedActor = nullptr;
public:
	SceneHierarchyPanel(const std::string& pName);
	~SceneHierarchyPanel();
	void Draw() override;
	void ResetSelectedActor();
	Actor* GetSelectedActor() const;
};
