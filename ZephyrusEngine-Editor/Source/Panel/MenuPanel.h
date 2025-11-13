#pragma once
#include "Panel.h"

class ImGuiEditorLayer;

class MenuPanel : public Panel
{
private:
	ImGuiEditorLayer* mApplication;
public:
	MenuPanel(ISceneContext* pSceneContext, const std::string& pName, ImGuiEditorLayer* pApplication);
	~MenuPanel();
	void Draw() override;
};
