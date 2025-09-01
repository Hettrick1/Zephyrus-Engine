#include "ConsolePanel.h"

ConsolePanel::ConsolePanel(const std::string& pName)
	: Panel(pName)
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
	Panel::BeginDraw();
	ImGui::Begin(mName.c_str());
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& message : mLogMessages)
    {
        ImVec4 color;
        switch (message.pType) {
        case Zephyrus::LogType::ZLT_INFO:
            color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case Zephyrus::LogType::ZLT_LOAD:
            color = ImVec4(0.0f, 0.8, 0.1f, 1.0f);
            break;
        case Zephyrus::LogType::ZLT_WARN:
            color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            break;
        case Zephyrus::LogType::ZLT_ERROR:
            color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case Zephyrus::LogType::ZLT_ASSERT:
            color = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);
            break;
        default:
            color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        }
        ImGui::TextColored(color, "%s", message.pTexte.c_str());
        ImGui::Separator();
    }
    if (mMessageQuantity < mLogMessages.size() && mNewMessage)
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

void ConsolePanel::OnLogMessage(const Zephyrus::ZPMessage& pMessage)
{
	if (pMessage.pLogger == Zephyrus::Logger::ZP_ZEPHYRUS)
	{
		return;
	}
	else
	{
		mLogMessages.push_back(pMessage);
        mNewMessage = true;
	}
}
