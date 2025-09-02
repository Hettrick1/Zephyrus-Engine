#pragma once
#include "Panel.h"

class MenuPanel : public Panel
{
private:

public:
	MenuPanel(const std::string& pName);
	~MenuPanel();
	void Draw() override;
};
