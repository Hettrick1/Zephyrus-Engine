#pragma once
#include "Panel.h"
#include <filesystem>

class ContentBrowserPanel : public Panel
{
private:

public:
	ContentBrowserPanel(const std::string& pName);
	~ContentBrowserPanel();
	void Draw() override;
	void DrawDirectory(const std::string& folderPath);
};
