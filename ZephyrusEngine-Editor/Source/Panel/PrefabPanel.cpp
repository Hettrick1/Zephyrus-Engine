#include "PrefabPanel.h"
#include "EditorUI/ImGuiUtils.h"
#include "Assets.h"
#include "SceneManager.h"
#include "PrefabFactory.h"
#include "../EditorApplication/EventSystem/EventSystem.h"
#include "../EditorApplication/EventSystem/Event/SpawnPrefabEvent.h"
#include "Interface/ITexture.h"

using Zephyrus::Assets::AssetsManager;

PrefabPanel::PrefabPanel(ISceneContext* pSceneContext, const std::string& pName)
	: Panel(pSceneContext, pName)
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

	Zephyrus::Assets::ITexture* arrowTex = AssetsManager::LoadTexture("Sprites/Icons/prefab36.png", "Sprites/Icons/prefab36.png");
	ImTextureID myIcon = (ImTextureID)(intptr_t)arrowTex->GetHandle();

	ImVec2 iconSize(36, 36);
	ImVec2 buttonSize(330, 48);

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 windowSize = ImGui::GetContentRegionAvail();

	pos.x += (windowSize.x * 0.5 - buttonSize.x * 0.5) + 5;
	pos.y += 20;

	ImGui::SetCursorScreenPos(pos);

	auto prefabList = mContext->GetPrefabFactory()->GetPrefabFiles("../Content/Prefabs");

	for (int i = 0; i < prefabList.size(); i++)
	{
		if (ZP::UI::ImageTextButton(myIcon, buttonSize, iconSize, pos, prefabList[i].c_str(), ZP::UI::gFonts.medium))
		{
			SpawnPrefabEvent* event = new SpawnPrefabEvent(mContext, prefabList[i].c_str());
			EventSystem::DoEvent(event);
		}
		pos.y += buttonSize.y + 10;
	}

	ImGui::End();
	Panel::EndDraw();
}
