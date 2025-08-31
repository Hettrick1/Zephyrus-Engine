#pragma once
#include "Panel.h"

class ContentBrowserPanel : public Panel
{
private:

public:
	ContentBrowserPanel(const std::string& pName);
	~ContentBrowserPanel();
	void Draw() override;
};
