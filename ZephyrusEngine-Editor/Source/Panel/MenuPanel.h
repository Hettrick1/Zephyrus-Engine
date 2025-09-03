#pragma once
#include "Panel.h"

class EditorApplication;

class MenuPanel : public Panel
{
private:
	EditorApplication* mApplication;
public:
	MenuPanel(const std::string& pName, EditorApplication* pApplication);
	~MenuPanel();
	void Draw() override;
};
