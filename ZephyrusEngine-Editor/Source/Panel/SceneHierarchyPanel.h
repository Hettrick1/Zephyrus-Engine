#pragma once
#include "Panel.h"

class SceneHierarchyPanel : public Panel
{
private:

public:
	SceneHierarchyPanel(const std::string& pName);
	~SceneHierarchyPanel();
	void Draw() override;
};
