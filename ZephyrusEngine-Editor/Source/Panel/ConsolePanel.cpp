#include "ConsolePanel.h"

ConsolePanel::ConsolePanel(ISceneContext* pSceneContext, const std::string& pName)
	: Panel(pSceneContext, pName)
{
}

ConsolePanel::~ConsolePanel()
{
	mLogMessages.clear();
}

void ConsolePanel::Update()
{
    if (mLogMessages.size() > mMaxMessageInConsole)
    {
        mLogMessages.pop_front();
    }
}

void ConsolePanel::Draw()
{
    if (!mDrawPanel)
    {
        return;
    }
    Panel::BeginDraw();
    ImGui::Begin(mName.c_str());

    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("MenuRegion1", ImVec2(100, 20), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(0, 0));
    if (ImGui::BeginMenu("Show Logs"))
    {
        if (ImGui::MenuItem("Infos", nullptr, mShowInfos)) 
        {
            mShowInfos = !mShowInfos;
            mNewMessage = true;
        }
        if (ImGui::MenuItem("Loads", nullptr, mShowLoads))
        {
            mShowLoads = !mShowLoads;
            mNewMessage = true;
        }
        if (ImGui::MenuItem("Warns", nullptr, mShowWarns))
        {
            mShowWarns = !mShowWarns;
            mNewMessage = true;
        }
        if (ImGui::MenuItem("Errors", nullptr, mShowErrors))
        {
            mShowErrors = !mShowErrors;
            mNewMessage = true;
        }
        ImGui::EndMenu();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("MenuRegion2", ImVec2(windowSize.x - 120, 20), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
    ImVec2 textSize = ImGui::CalcTextSize("Clear All");
    ImGui::SetCursorPos(ImVec2(windowSize.x - textSize.x - 130, 0));
    if (ImGui::Button("Clear All"))
    {
        mLogMessages.clear();
        mMessageQuantity = 0;
    }
    ImGui::EndChild();

    ImGui::Separator();

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& message : mLogMessages)
    {
        ImVec4 color;
        switch (message.pType) {
        case Zephyrus::Debug::LogType::ZLT_INFO:
            color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case Zephyrus::Debug::LogType::ZLT_LOAD:
            color = ImVec4(0.0f, 0.8, 0.1f, 1.0f);
            break;
        case Zephyrus::Debug::LogType::ZLT_WARN:
            color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            break;
        case Zephyrus::Debug::LogType::ZLT_ERROR:
            color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case Zephyrus::Debug::LogType::ZLT_ASSERT:
            color = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);
            break;
        default:
            color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        }

        if (message.pType == Zephyrus::Debug::LogType::ZLT_INFO && !mShowInfos
            || message.pType == Zephyrus::Debug::LogType::ZLT_LOAD && !mShowLoads
            || message.pType == Zephyrus::Debug::LogType::ZLT_WARN && !mShowWarns
            || message.pType == Zephyrus::Debug::LogType::ZLT_ERROR && !mShowErrors)
        {
            continue;
        }

        ImGui::TextColored(color, "%s", message.pTexte.c_str());
        ImGui::Separator();
    }
    if (mMessageQuantity < mLogMessages.size() || mNewMessage)
    {
        mMessageQuantity = mLogMessages.size();
        ImGui::SetScrollHereY(1.0f);
        mNewMessage = false;
    }

    if (mMessageQuantity > mLogMessages.size() && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 5.0f)
    {
        mMessageQuantity = mLogMessages.size() - 1;
    }
    ImGui::EndChild();
	ImGui::End();
	Panel::EndDraw();

}

void ConsolePanel::OnLogMessage(const Zephyrus::Debug::ZPMessage& pMessage)
{
	if (pMessage.pLogger == Zephyrus::Debug::Logger::ZP_ZEPHYRUS)
	{
		return;
	}
	else
	{
		mLogMessages.push_back(pMessage);
        mNewMessage = true;
	}
}
