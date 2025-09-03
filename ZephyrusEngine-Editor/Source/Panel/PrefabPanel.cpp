#include "PrefabPanel.h"
#include "EditorUI/ImGuiUtils.h"
#include "Assets.h"
#include "PrefabFactory.h"

PrefabPanel::PrefabPanel(const std::string& pName)
	: Panel(pName)
{
}

PrefabPanel::~PrefabPanel()
{
}

void PrefabPanel::Draw()
{
	if (!mDrawPanel)
	{
		return;
	}
	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());

	Texture* arrowTex = Assets::LoadTexture("Sprites/Icons/prefab48.png", "Sprites/Icons/prefab48.png");
	ImTextureID myIcon = (ImTextureID)(intptr_t)arrowTex->GetId();

	ImVec2 iconSize(48, 48);
	ImVec2 buttonSize(360, 64);

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 windowSize = ImGui::GetContentRegionAvail();

	pos.x += (windowSize.x * 0.5 - buttonSize.x * 0.5) + 5;
	pos.y += 20;

	ImGui::SetCursorScreenPos(pos);

	auto prefabList = PrefabFactory::GetPrefabFiles("../Content/Prefabs");

	for (int i = 0; i < prefabList.size(); i++)
	{
		ZP::UI::ImageTextButton(myIcon, buttonSize, iconSize, pos, prefabList[i].c_str(), ZP::UI::gFonts.medium);
		pos.y += buttonSize.y + 10;
	}

	ImGui::End();
	Panel::EndDraw();
}
