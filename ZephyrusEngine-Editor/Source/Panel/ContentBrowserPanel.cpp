#include "ContentBrowserPanel.h"
#include "Texture.h"
#include "Assets.h"
#include "SceneManager.h"
#include "HudManager.h"
#include "../EditorApplication/EventSystem/EventSystem.h"
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

std::filesystem::path ContentBrowserPanel::rootDirectory = "../Content";
std::filesystem::path ContentBrowserPanel::currentDirectory = rootDirectory;

bool isSelected;
std::filesystem::path selectedEntry;

using Zephyrus::Assets::AssetsManager;

ContentBrowserPanel::ContentBrowserPanel(ISceneContext* pSceneContext, const std::string& pName)
    : Panel(pSceneContext, pName)
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
        float height = ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(currentDirectory.string().c_str()).y;
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
        DrawDirectoryContent(currentDirectory);
        ImGui::EndChild();
        ImGui::PopStyleVar();

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
        {
            ImGui::OpenPopup("CreationPopUP");
        }

        if (ImGui::BeginPopup("CreationPopUP"))
        {
            if (ImGui::BeginMenu("Add"))
            {
                if (ImGui::MenuItem("New Map"))
                {
                    std::filesystem::path newMapPath = currentDirectory / "NewMap.zpmap";

                    int counter = 1;
                    while (std::filesystem::exists(newMapPath))
                    {
                        newMapPath = currentDirectory / ("NewMap" + std::to_string(counter) + ".zpmap");
                        counter++;
                    }

                    std::ofstream file(newMapPath);
                    if (file.is_open())
                    {
                        file << "{}";
                        file.close();
                    }
                }
                ImGui::EndMenu();
            }
            if (!selectedEntry.empty())
            {
                if (ImGui::MenuItem("Delete"))
                {
                    DeleteFileOrDirectory();
                }
            }
            ImGui::EndPopup();
        }

        if (!selectedEntry.empty() && ImGui::IsKeyPressed(ImGuiKey_Delete) && ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        {
            DeleteFileOrDirectory();
        }

        CreatePrefabFile(currentDirectory.string());

        ImGui::Text(currentDirectory.string().c_str());
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
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

            bool nodeOpen = ImGui::TreeNodeEx(entry.path().filename().string().c_str(), flags);

            if (ImGui::IsItemClicked() && nodeOpen)
            {
                currentDirectory = entry.path();
            }
            else if (ImGui::IsItemClicked() && !nodeOpen)
            {
                currentDirectory = entry.path().parent_path();
            }
            if (nodeOpen)
            {
                DrawDirectory(entry.path().string());
                ImGui::TreePop();
            }
        }
    }
}

void ContentBrowserPanel::DrawDirectoryContent(const std::filesystem::path& directory)
{
    int columns = 12;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
    ImGui::Columns(columns, 0, false);

    if (currentDirectory != rootDirectory)
    {
        isSelected = (selectedEntry == currentDirectory.parent_path());
        ImageButton(isSelected, "folder", "folder");
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            if (ImGui::IsItemClicked())
            {
                selectedEntry.clear();
            }
            else if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
            {
                selectedEntry.clear();
            }
        }

        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
        {
            currentDirectory = currentDirectory.parent_path();
            selectedEntry.clear();
        }
        ImGui::TextWrapped("...");

        ImGui::NextColumn();
    }

    //first directories
    for (auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_directory())
        {
            DrawEntry(entry);
        }
    }

    //then files
    for (auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (!entry.is_directory() && entry.path().extension().string() != ".txt" && entry.path().extension().string() != ".zip")
        {
            DrawEntry(entry);
        }
    }

    ImGui::Columns(1);
    ImGui::PopStyleVar();
}

void ContentBrowserPanel::DrawEntry(const std::filesystem::directory_entry& entry)
{
    const auto& path = entry.path();

    isSelected = (selectedEntry == path);

    std::string name = path.filename().string();

    ImGui::PushID(name.c_str());

    ImageButton(isSelected, entry.path().string(), entry.path().filename().extension().string());

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        if (ImGui::IsItemClicked())
        {
            selectedEntry = path;
        }
        else if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
        {
            selectedEntry.clear();
        }
    }

    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
    {
        if (entry.is_directory())
        {
            currentDirectory = path;
            selectedEntry.clear();
        }
        else
        {
            if (entry.path().extension().string() != ".zpmap")
            {
#ifdef _WIN32
                ShellExecuteA(nullptr, "open", path.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#endif
            }
            else // load map
            {
                std::filesystem::path fsPath = path.lexically_normal();
                std::string normalizedPath = fsPath.generic_string();
                mContext->LoadSceneWithFile(normalizedPath, nullptr, false);
                mContext->SetSceneLoaded(true);
                mContext->GetRenderer()->GetHud()->Unload();
                mHierarchy->ResetSelectedActor();
                EventSystem::ClearAllEvents();
                resetfunc();
            }
        }
    }

    float padding = 6.0f;
    float boxSize = 80.0f;
    float wrapWidth = boxSize - 2 * padding;

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImVec2 textSize = ImGui::CalcTextSize(name.c_str(), nullptr, true, wrapWidth * 1.8);

    if (isSelected)
    {
        ImU32 bgColor = IM_COL32(70, 70, 70, 255);
        ImGui::GetWindowDrawList()->AddRectFilled(
            pos,
            ImVec2(pos.x + boxSize, pos.y + textSize.y + 2 * padding),
            bgColor,
            4.0f
        );
    }

    ImGui::TextWrapped(name.c_str());

    ImGui::NextColumn();

    ImGui::PopID();
}

void ContentBrowserPanel::ImageButton(bool pIsSelected, const std::string& entry, const std::string& extension)
{
    ImVec2 size(80, 80);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 end = ImVec2(pos.x + size.x, pos.y + size.y);

    std::filesystem::path p(entry);
    std::string cleanPath = p.lexically_normal().generic_string();

    ImGui::InvisibleButton(("##" + cleanPath).c_str(), size); // TODO clean the dropsource data function -> it's messy rn
    if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload("TEXTURE", cleanPath.c_str(), cleanPath.size() + 1);
            ImGui::Text(cleanPath.c_str());
            ImGui::EndDragDropSource();
        }
    }
    else if (extension == ".obj" || extension == ".fbx")
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload("MESH", cleanPath.c_str(), cleanPath.size() + 1);
            ImGui::Text(cleanPath.c_str());
            ImGui::EndDragDropSource();
        }
    }
    else if (extension == ".prefab")
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload("PREFAB", cleanPath.c_str(), cleanPath.size() + 1);
            ImGui::Text(cleanPath.c_str());
            ImGui::EndDragDropSource();
        }
    }

    ImU32 bgColor;

    if (ImGui::IsItemActive())
    {
        bgColor = IM_COL32(100, 100, 100, 255);
    }
    else if (ImGui::IsItemHovered())
    {
        bgColor = IM_COL32(140, 140, 140, 255);
    }
    else if (pIsSelected)
    {
        bgColor = IM_COL32(70, 70, 70, 255);
    }
    else
    {
        bgColor = IM_COL32(0, 0, 0, 0);
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(
        pos,
        end,
        bgColor,
        8.0f
    );

    ImTextureID myIcon = GetImageFromExtension(extension, entry);

    draw_list->AddImage(
        myIcon,
        pos,
        end
    );
}

ImTextureID ContentBrowserPanel::GetImageFromExtension(const std::string& extension, std::string filepath)
{
    Zephyrus::Assets::Texture* tex;
    if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
    {
        tex = AssetsManager::LoadTexture(filepath, filepath);
    }
    else if (extension == ".otf" || extension == ".ttf")
    {
        tex = AssetsManager::LoadTexture("../Content/Sprites/Icons/font80.png", "../Content/Sprites/Icons/font80.png");
    }
    else if (extension == ".zpmap")
    {
        tex = AssetsManager::LoadTexture("../Content/Sprites/Icons/scene80.png", "../Content/Sprites/Icons/scene80.png");
    }
    else if (extension == ".obj" || extension == ".fbx")
    {
        tex = AssetsManager::LoadTexture("../Content/Sprites/Icons/mesh80.png", "../Content/Sprites/Icons/mesh80.png");
    }
    else if (extension == ".prefab")
    {
        tex = AssetsManager::LoadTexture("../Content/Sprites/Icons/prefab80.png", "../Content/Sprites/Icons/prefab80.png");
    }
    else if (extension == ".vert" || extension == ".frag" || extension == ".tesc" || extension == ".tese" || extension == ".geom")
    {
        tex = AssetsManager::LoadTexture("../Content/Sprites/Icons/shader80.png", "../Content/Sprites/Icons/shader80.png");
    }
    else
    {
        tex = AssetsManager::LoadTexture("../Content/Sprites/Icons/folder80.png", "../Content/Sprites/Icons/folder80.png");
    }
    ImTextureID myIcon = (ImTextureID)(intptr_t)tex->GetId();

    return myIcon;
}

void ContentBrowserPanel::SetSceneHierarchy(SceneHierarchyPanel* pHierarchy)
{
    mHierarchy = pHierarchy;
}

void ContentBrowserPanel::DeleteFileOrDirectory()
{
    if (std::filesystem::is_directory(selectedEntry))
    {
        std::filesystem::remove_all(selectedEntry);
    }
    else
    {
        std::filesystem::remove(selectedEntry);
    }
    selectedEntry.clear();
}

void ContentBrowserPanel::CreatePrefabFile(const std::string& pFilepath)
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ACTOR"))
        {
            std::string actorID((const char*)payload->Data, payload->DataSize);

            auto actor = mContext->GetActiveScene()->GetActorWithID(actorID);

            if (!actor)
            {
                return;
            }
            std::filesystem::path newPrefabPath = currentDirectory / (actor->GetName() + ".prefab");

            int counter = 1;
            while (std::filesystem::exists(newPrefabPath))
            {
                newPrefabPath = currentDirectory / (actor->GetName() + std::to_string(counter) + ".prefab");
                counter++;
            }

            actor->SerializePrefab(newPrefabPath.string());
        }
        ImGui::EndDragDropTarget();
    }
}
