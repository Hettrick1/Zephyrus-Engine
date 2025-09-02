#pragma once
#include "Panel.h"

class UtilsPanel : public Panel
{
private:

public:
	UtilsPanel(const std::string& pName);
	~UtilsPanel();
	void Draw() override;
};
