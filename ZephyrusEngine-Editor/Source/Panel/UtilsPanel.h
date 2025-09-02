#pragma once
#include "Panel.h"

enum class LaunchGameMode
{
	Standalone,
	Editor
};

class UtilsPanel : public Panel
{
private:
	float mPaneSizeY = 0;
	LaunchGameMode mSelectedOption = LaunchGameMode::Standalone;
	ImTextureID mMyIcon = 0;
public:
	UtilsPanel(const std::string& pName, float pPanelSizeY);
	~UtilsPanel();
	void Draw() override;
};
