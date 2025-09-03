#include "UtilsPanel.h"
#include "Texture.h"
#include "Assets/Assets.h"
#include <cstdlib>
#include <windows.h>

UtilsPanel::UtilsPanel(const std::string& pName, float pPanelSizeY)
	: Panel(pName), mPaneSizeY(pPanelSizeY)
{
}

UtilsPanel::~UtilsPanel()
{
}

void UtilsPanel::Draw()
{
    BeginDrawPanelWindow();

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImVec2 buttonSize = ImVec2(200, mPaneSizeY - 15);

    DrawPlayButon(windowSize, buttonSize);

    ImGui::SameLine(0.f, 0.f);

    DrawDropDownButton(windowSize);

    ImGui::Separator();

    EndDrawPanelWindow();
}

void UtilsPanel::BeginDrawPanelWindow()
{
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0));
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, mPaneSizeY));

    ImGui::Begin("CenterPanel", nullptr, flags);
}

void UtilsPanel::EndDrawPanelWindow()
{
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);
}

void UtilsPanel::DrawPlayButon(const ImVec2& pWindowSize, const ImVec2& pButtonSize)
{
    ImVec2 pos = ImVec2((pWindowSize.x - pButtonSize.x - 20) * 0.5f, (pWindowSize.y - pButtonSize.y) * 0.5f + (15 * 0.5f));
    ImGui::SetCursorPos(pos);

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.81176f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);

    std::string selectedOptionText;

    switch (mSelectedOption)
    {
    case LaunchGameMode::Standalone:
        selectedOptionText = "Play in Standalone";
        break;
    case LaunchGameMode::Editor:
        selectedOptionText = "Play in Editor";
        break;
    default:
        selectedOptionText = "Play";
        break;
    }

    if (ImGui::Button(selectedOptionText.c_str(), pButtonSize))
    {
        LaunchGame();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
}

void UtilsPanel::DrawDropDownButton(const ImVec2& pWindowSize)
{
    float x = ImGui::GetCursorPosX();
    float y = ImGui::GetCursorPosY();

    ImVec2 size(20, 30);
    ImVec2 popupPos;

    popupPos.x = pWindowSize.x * 0.5 - 30;
    popupPos.y = pWindowSize.y + 54.5;

    Texture* arrowTex = Assets::LoadTexture("Sprites/Icons/arrowDown.png", "Sprites/Icons/arrowDown.png");
    mMyIcon = (ImTextureID)(intptr_t)arrowTex->GetId();
    ImVec2 iconSize(24, 24);

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.81176f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);

    bool pressed = ImGui::ImageButton("arrow_btn", mMyIcon, iconSize);

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);

    if (pressed)
    {
        ImGui::OpenPopup("PlayMode");
        ImGui::SetNextWindowPos(popupPos);
    }

    DrawDropMenuOptions();
}

void UtilsPanel::DrawDropMenuOptions()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    if (ImGui::BeginPopup("PlayMode"))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
        if (ImGui::MenuItem("Play in Standalone", nullptr, mSelectedOption == LaunchGameMode::Standalone))
        {
            mSelectedOption = LaunchGameMode::Standalone;
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Play in Editor", nullptr, mSelectedOption == LaunchGameMode::Editor))
        {
            mSelectedOption = LaunchGameMode::Editor;
        }
        ImGui::PopStyleVar();
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
}

void UtilsPanel::LaunchGame()
{
    switch (mSelectedOption)
    {
    case LaunchGameMode::Standalone:
    {
        char path[MAX_PATH];
        GetModuleFileNameA(nullptr, path, MAX_PATH);
        std::string exePath(path);
        size_t pos = exePath.find_last_of("\\/");
        std::string finalPath = exePath.substr(0, pos);

        std::string runtimePath = finalPath + std::string("\\..\\ZephyrusEngine-Runtime\\ZephyrusEngine-Runtime.exe");

        system(runtimePath.c_str());
    }
    break;
    case LaunchGameMode::Editor:
        ZP_EDITOR_WARN("Not Implemented Yet !");
        break;
    default:
        ZP_EDITOR_ERROR("No launch option selected !");
        break;
    }
}