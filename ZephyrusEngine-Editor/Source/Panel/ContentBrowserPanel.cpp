#include "ContentBrowserPanel.h"

ContentBrowserPanel::ContentBrowserPanel(const std::string& pName)
	: Panel(pName)
{
}

ContentBrowserPanel::~ContentBrowserPanel()
{
}

void ContentBrowserPanel::Draw()
{
	if (!mDrawPanel)
	{
		return;
	}

	Panel::BeginDraw();
    if (ImGui::Begin("Content Browser"))
    {
        static float width = 200.0f;
        static float height = 210.0f;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::BeginChild("child1", ImVec2(width, height), true);
        DrawDirectory("../Content");
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, height));
        if (ImGui::IsItemActive())
        {
            width += ImGui::GetIO().MouseDelta.x;
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        }

        ImGui::SameLine();

        ImGui::BeginChild("child2", ImVec2(0, height), true);

        ImGui::EndChild();
        ImGui::PopStyleVar();
    }
    ImGui::End();
	Panel::EndDraw();
}

void ContentBrowserPanel::DrawDirectory(const std::string& folderPath)
{
    for (auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.is_directory())
        {
            if (ImGui::TreeNode(entry.path().filename().string().c_str()))
            {
                DrawDirectory(entry.path().string());
                ImGui::TreePop();
            }
        }
    }
}
