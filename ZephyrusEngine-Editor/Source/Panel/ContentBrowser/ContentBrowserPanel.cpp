#include "ContentBrowserPanel.h"
#include "Interface/ITexture2D.h"
#include "AssetsManager.h"
#include "SceneManager.h"
#include "HudManager.h"
#include "../../EditorApplication/EventSystem/EventSystem.h"
#include "Window/WindowManager.h"
#include "Window/MaterialWindow/MaterialWindow.h"
#include "../../EditorUI/ImGuiUtils.h"
#include "FileAsset.h"
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

std::filesystem::path ContentBrowserPanel::mRootDirectory = "../Content";
std::filesystem::path ContentBrowserPanel::mCurrentDirectory = mRootDirectory;

using Zephyrus::Assets::AssetsManager;

ContentBrowserPanel::ContentBrowserPanel(ISceneContext* pSceneContext, const std::string& pName, std::shared_ptr<Zephyrus::Editor::Window::WindowManager> pWindowManager)
    : Panel(pSceneContext, pName), mWindowManager(pWindowManager)
{
    auto database = AssetsManager::GetInstance().GetDatabase();
    for (auto [id, path] : database.GetContent())
    {
        FileAsset asset;
        asset.mId = id;
        asset.mPath = path;
        asset.mExtension = std::filesystem::path(path).extension().string();
        asset.mMetaPath = path + ".meta";
        asset.mType = GetTypeFromExtension(asset.mExtension);
        
        mFileAssets.emplace(path, asset);
    }
    RefreshContentBrowser(mRootDirectory);
}

void ContentBrowserPanel::RefreshContentBrowser(std::filesystem::path& path)
{
    mCurrentItemsInFolder.clear();
    std::vector<ContentBrowserItem> files;
    for (auto& entry : std::filesystem::directory_iterator(path))
    {
        ContentBrowserItem item;
        item.path = entry.path();
        item.isDirectory = entry.is_directory();

        if (item.isDirectory)
        {
            mCurrentItemsInFolder.push_back(item);
        }
        else
        {
            auto extension = entry.path().extension();
            if (extension != ".zip" && extension != ".txt" && extension != ".meta")
            {
                auto file = mFileAssets.find(item.path.make_preferred().string());
                item.asset = file == mFileAssets.end() ? nullptr : &file->second;
                files.emplace_back(item);
            }
        }
    }
    mCurrentItemsInFolder.insert(mCurrentItemsInFolder.end(), files.begin(), files.end());
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
        ImGui::Text(mCurrentDirectory.string().c_str());
        static float width = 200.0f;
        float height = ImGui::GetContentRegionAvail().y;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::BeginChild("child1", ImVec2(width, height), true);
        DrawDirectory("../Content");
        ImGui::EndChild();

        ImGui::SameLine();

        ZP::UI::DrawVerticalSplitterButton(width);

        ImGui::SameLine();

        ImGui::BeginChild("child2", ImVec2(0, height), true);
        DrawDirectoryContent();
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
                    std::filesystem::path newMapPath = mCurrentDirectory / "NewMap.zpmap";

                    int counter = 1;
                    while (std::filesystem::exists(newMapPath))
                    {
                        newMapPath = mCurrentDirectory / ("NewMap" + std::to_string(counter) + ".zpmap");
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
            if (!mSelectedEntry.empty())
            {
                if (ImGui::MenuItem("Delete"))
                {
                    DeleteFileOrDirectory();
                }
            }
            ImGui::EndPopup();
        }

        if (!mSelectedEntry.empty() && ImGui::IsKeyPressed(ImGuiKey_Delete) && ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        {
            DeleteFileOrDirectory();
        }

        CreatePrefabFile(mCurrentDirectory.string());
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
                mCurrentDirectory = entry.path();
            }
            else if (ImGui::IsItemClicked() && !nodeOpen)
            {
                mCurrentDirectory = entry.path().parent_path();
            }
            if (nodeOpen)
            {
                DrawDirectory(entry.path().string());
                ImGui::TreePop();
            }
        }
    }
}

void ContentBrowserPanel::DrawDirectoryContent()
{
    int columns = 12;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
    ImGui::Columns(columns, 0, false);

    if (mCurrentDirectory != mRootDirectory)
    {
        mIsSelected = (mSelectedEntry == mCurrentDirectory.parent_path());
        ContentBrowserItem returnFolder;
        returnFolder.path = mCurrentDirectory;
        returnFolder.isDirectory = true;
        returnFolder.asset = nullptr;
        ImageButton(mIsSelected, returnFolder);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            if (ImGui::IsItemClicked() || (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()))
            {
                mSelectedEntry.clear();
            }
        }

        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
        {
            mCurrentDirectory = mCurrentDirectory.parent_path();
            mSelectedEntry.clear();
            RefreshContentBrowser(mCurrentDirectory);
        }
        ImGui::TextWrapped("...");

        ImGui::NextColumn();
    }

    for (auto item : mCurrentItemsInFolder)
    {
        DrawItem(item);
    }

    ImGui::Columns(1);
    ImGui::PopStyleVar();
}

void ContentBrowserPanel::DrawItem(ContentBrowserItem& item)
{
    mIsSelected = (mSelectedEntry == item.path);

    std::string name = item.path.filename().string();

    ImGui::PushID(name.c_str());

    ImageButton(mIsSelected, item);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        if (ImGui::IsItemClicked())
        {
            mSelectedEntry = item.path.string();
        }
        else if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
        {
            mSelectedEntry.clear();
        }
    }

    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
    {
        if (item.isDirectory)
        {
            mCurrentDirectory = item.path;
            mSelectedEntry.clear();
            RefreshContentBrowser(mCurrentDirectory);
        }
        else // TODO : Create a map and function to open files, if the extension is not found then open with shellexecuteA
        {
            if (item.asset->mType != FileType::Material && item.asset->mType != FileType::Map)
            {
#ifdef _WIN32
                ShellExecuteA(nullptr, "open", item.path.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#endif
            }
            else if (item.asset->mType == FileType::Map)// load map
            {
                std::filesystem::path fsPath = item.path.lexically_normal();
                std::string normalizedPath = fsPath.generic_string();
                mContext->LoadSceneWithFile(normalizedPath, nullptr, false);
                mContext->SetSceneLoaded(true);
                mContext->GetRenderer()->GetHud()->Unload();
                mHierarchy->ResetSelectedActor();
                EventSystem::ClearAllEvents();
                resetfunc();
            }
            else if (item.asset->mType == FileType::Material)
            {
                std::filesystem::path fsPath = item.path.lexically_normal();
                std::string fileName = fsPath.filename().string();
                std::string normalizedPath = fsPath.generic_string();
                mWindowManager->OpenWindow<Zephyrus::Editor::Window::MaterialWindow>(normalizedPath, fileName);
            }
        }
    }

    float padding = 6.0f;
    float boxSize = 80.0f;
    float wrapWidth = boxSize - 2 * padding;

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImVec2 textSize = ImGui::CalcTextSize(name.c_str(), nullptr, true, wrapWidth * 1.8);

    if (mIsSelected)
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

void ContentBrowserPanel::ImageButton(bool pIsSelected, const ContentBrowserItem& file)
{
    ImVec2 size(80, 80);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 end = ImVec2(pos.x + size.x, pos.y + size.y);
    
    std::string cleanPath = file.path.lexically_normal().generic_string();
    auto asset = file.asset;
    
    ImGui::InvisibleButton(("##" + cleanPath).c_str(), size);

    if (asset) // TODO : This could be in a function
    {
        switch (asset->mType)
        {
        case FileType::Image:
            CreateDragDropSource("TEXTURE", file);
            break;
        case FileType::Mesh:
            CreateDragDropSource("MESH", file);
            break;
        case FileType::Prefab:
            CreateDragDropSource("PREFAB", file);
            break;
        case FileType::Shader:
            CreateDragDropSource("SHADER", file);
            break;
        case FileType::Material:
            CreateDragDropSource("MATERIAL", file);
            break;
        case FileType::Map:
            CreateDragDropSource("MAP", file);
        case FileType::Font:
            CreateDragDropSource("FONT", file);
            break;
        case FileType::None:
            break;
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

    ImTextureID myIcon;
    if (asset)
    {
        myIcon = GetImageFromType(asset->mType, file.path.lexically_normal().generic_string());
    }
    else
    {
        myIcon = (ImTextureID)(intptr_t)AssetsManager::GetInstance().LoadTexture("../Content/Sprites/Icons/folder80.png", "../Content/Sprites/Icons/folder80.png")->GetHandle();
    }

    draw_list->AddImage(
        myIcon,
        pos,
        end
    );
}

void ContentBrowserPanel::CreateDragDropSource(const std::string& name, const ContentBrowserItem& data)
{
    if (!data.asset)
    {
        return;
    }
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        ImGui::SetDragDropPayload(name.c_str(), data.asset->mId.c_str(), data.asset->mId.size());
        ImGui::Text(data.path.string().c_str());
        ImGui::EndDragDropSource();
    }
}

ImTextureID ContentBrowserPanel::GetImageFromType(const FileType& type, const std::string& filepath)
{
    Zephyrus::Assets::ITexture2D* tex;

    switch (type)
    {
    case FileType::Image:
        tex = AssetsManager::GetInstance().LoadTexture(filepath, filepath);
        break;
    case FileType::Font:
        tex = AssetsManager::GetInstance().LoadTexture("../Content/Sprites/Icons/font80.png", "../Content/Sprites/Icons/font80.png");
        break;
    case FileType::Mesh:
        tex = AssetsManager::GetInstance().LoadTexture("../Content/Sprites/Icons/mesh80.png", "../Content/Sprites/Icons/mesh80.png");
        break;
    case FileType::Prefab:
        tex = AssetsManager::GetInstance().LoadTexture("../Content/Sprites/Icons/prefab80.png", "../Content/Sprites/Icons/prefab80.png");
        break;
    case FileType::Shader:
        tex = AssetsManager::GetInstance().LoadTexture("../Content/Sprites/Icons/shader80.png", "../Content/Sprites/Icons/shader80.png");
        break;
    case FileType::Material:
        tex = AssetsManager::GetInstance().LoadTexture("../Content/Sprites/Icons/mat80.png", "../Content/Sprites/Icons/mat80.png");
        break;
    case FileType::Map:
        tex = AssetsManager::GetInstance().LoadTexture("../Content/Sprites/Icons/scene80.png", "../Content/Sprites/Icons/scene80.png");
        break;
    default:
        tex = AssetsManager::GetInstance().LoadTexture("../Content/Sprites/Icons/folder80.png", "../Content/Sprites/Icons/folder80.png");
        break;
    }
    ImTextureID myIcon = (ImTextureID)(intptr_t)tex->GetHandle();

    return myIcon;
}

void ContentBrowserPanel::SetSceneHierarchy(SceneHierarchyPanel* pHierarchy)
{
    mHierarchy = pHierarchy;
}

void ContentBrowserPanel::DeleteFileOrDirectory()
{
    if (std::filesystem::is_directory(mSelectedEntry))
    {
        std::filesystem::remove_all(mSelectedEntry);
    }
    else
    {
        std::filesystem::remove(mSelectedEntry);
    }
    mSelectedEntry.clear();
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
            std::filesystem::path newPrefabPath = mCurrentDirectory / (actor->GetName() + ".prefab");

            int counter = 1;
            while (std::filesystem::exists(newPrefabPath))
            {
                newPrefabPath = mCurrentDirectory / (actor->GetName() + std::to_string(counter) + ".prefab");
                counter++;
            }

            actor->SerializePrefab(newPrefabPath.string());
        }
        ImGui::EndDragDropTarget();
    }
}
