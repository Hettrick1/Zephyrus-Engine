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

	void BeginDrawPanelWindow();
	void EndDrawPanelWindow();
	void DrawPlayButon(const ImVec2& pWindowSize, const ImVec2& pButtonSize);
	void DrawDropDownButton(const ImVec2& pWindowSize);
	void DrawDropMenuOptions();

	void LaunchGame();
};
