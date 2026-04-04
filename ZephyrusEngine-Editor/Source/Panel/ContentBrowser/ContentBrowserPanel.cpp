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
#include "Utils.h"
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>

#include <algorithm>
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
        CreateFileAsset(id, path);
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
        item.mPath = entry.path();
        item.mIsDirectory = entry.is_directory();

        if (item.mIsDirectory)
        {
            mCurrentItemsInFolder.push_back(item);
        }
        else
        {
            auto extension = entry.path().extension();
            if (extension != ".zip" && extension != ".txt" && extension != ".meta")
            {
                auto file = mFileAssets.find(item.mPath.make_preferred().string());
                item.mAsset = file == mFileAssets.end() ? nullptr : &file->second;
                files.emplace_back(item);
            }
        }
    }
    mCurrentItemsInFolder.insert(mCurrentItemsInFolder.end(), files.begin(), files.end());

    // refresh bread crumb bar
    auto currentPath = path;
    mPreviousFoldersInHierarchy.clear();
    for (int i = 0; i < 5; ++i)
    {
        if (currentPath != mRootDirectory.parent_path())
        {
            mPreviousFoldersInHierarchy.push_back(currentPath);
            currentPath = currentPath.parent_path();
        }
        else
        {
            break;
        }
    }
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
        float height = ImGui::GetContentRegionAvail().y;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::BeginChild("child1", ImVec2(width, height), true);
        DrawDirectoryTree("../Content");
        ImGui::EndChild();

        ImGui::SameLine();

        ZP::UI::DrawVerticalSplitterButton(width);

        ImGui::SameLine();

        ImGui::PopStyleVar();
        ImGui::BeginChild("filePool", ImVec2(0, height));
        
        DrawBrowserUtils(width);
        
        ImGui::BeginChild("child2", ImVec2(0, 0), ImGuiChildFlags_Borders);
        DrawDirectoryContent();
        ImGui::EndChild();
        ImGui::EndChild();

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

                    auto id = AssetsManager::GetInstance().GetDatabase().CreateMetaFromFile(newMapPath);
                    CreateFileAsset(id, newMapPath.make_preferred().string());
                    mNeedRefresh = true;
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

void ContentBrowserPanel::DrawDirectoryTree(const std::string& folderPath)
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
                DrawDirectoryTree(entry.path().string());
                ImGui::TreePop();
            }
        }
    }
}

void ContentBrowserPanel::DrawDirectoryContent()
{
    int columns = ImGui::GetContentRegionAvail().x / 120;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

    // TODO : USE TABLES
    ImGui::Columns(columns, 0, false);

    if (mCurrentDirectory != mRootDirectory)
    {
        mIsSelected = (mSelectedEntry == mCurrentDirectory.parent_path());
        ContentBrowserItem returnFolder;
        returnFolder.mPath = mCurrentDirectory;
        returnFolder.mIsDirectory = true;
        returnFolder.mAsset = nullptr;
        ImageButton(mIsSelected, returnFolder, "...");
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
            mNeedRefresh = true;
        }

        ImGui::NextColumn();
    }

    if (mNeedRefresh)
    {
        RefreshContentBrowser(mCurrentDirectory);
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
    mIsSelected = (mSelectedEntry == item.mPath);

    std::string name = item.mPath.filename().replace_extension("").string();

    ImGui::PushID(name.c_str());

    ImageButton(mIsSelected, item, name);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        if (ImGui::IsItemClicked())
        {
            mSelectedEntry = item.mPath.string();
        }
        else if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
        {
            mSelectedEntry.clear();
        }
    }

    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
    {
        if (item.mIsDirectory)
        {
            mCurrentDirectory = item.mPath;
            mSelectedEntry.clear();
            mNeedRefresh = true;
        }
        else // TODO : Create a map and function to open files, if the extension is not found then open with shellexecuteA
        {
            // ------------ EXAMPLE OF RENAME --------------
            
            // auto oldPath = item.asset->mPath;
            // item.asset->Rename("test");
            // auto newPath = item.asset->mPath;
            // auto browserItem = mFileAssets[oldPath];
            // mFileAssets.erase(oldPath);
            // mFileAssets[newPath] = browserItem;
            // mNeedRefresh = true;
            //RefreshContentBrowser(mCurrentDirectory);
            
            if (item.mAsset->mType != FileType::Material && item.mAsset->mType != FileType::Map)
            {
#ifdef _WIN32
                ShellExecuteA(nullptr, "open", item.mPath.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#endif
            }
            else if (item.mAsset->mType == FileType::Map)// load map
            {
                std::filesystem::path fsPath = item.mPath.lexically_normal();
                std::string normalizedPath = fsPath.generic_string();
                mContext->LoadSceneWithFile(normalizedPath, nullptr, false);
                mContext->SetSceneLoaded(true);
                mContext->GetRenderer()->GetHud()->Unload();
                mHierarchy->ResetSelectedActor();
                EventSystem::ClearAllEvents();
                resetfunc();
            }
            else if (item.mAsset->mType == FileType::Material)
            {
                std::filesystem::path fsPath = item.mPath.lexically_normal();
                std::string fileName = fsPath.filename().string();
                std::string id = item.mAsset->mId;
                mWindowManager->OpenWindow<Zephyrus::Editor::Window::MaterialWindow>(id, fileName);
            }
        }
    }

    ImGui::NextColumn();

    ImGui::PopID();
}

void ContentBrowserPanel::DrawBrowserUtils(float width)
{
    if (ImGui::BeginChild("Utils", ImVec2(0, 0), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders))
    {
        ImGui::Button("+ Add");
        
        ImGui::SameLine();
        
        ImGui::Button("Save All");

        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 5);
        ImGui::SameLine();

        float totalWidth = ImGui::GetContentRegionAvail().x;
        
        if (!mShowFullPath)
        {
            DrawBreadCrumb(totalWidth * 0.7f);
        }
        else
        {
            char fullPathBuffer[128];
            strncpy_s(fullPathBuffer, mCurrentDirectory.generic_string().c_str(), sizeof(fullPathBuffer));
            ImGui::SetNextItemWidth(totalWidth * 0.7f);
            ImGui::SetKeyboardFocusHere(0);
            ImGui::InputText("##BreadcrumbBar", fullPathBuffer, sizeof(fullPathBuffer), ImGuiInputTextFlags_AutoSelectAll);
            if (ImGui::IsItemDeactivated())
            {
                mShowFullPath = false;
            }
            if (ImGui::IsItemDeactivatedAfterEdit() && std::filesystem::exists(fullPathBuffer) && IsSubpathOf(fullPathBuffer, mRootDirectory))
            {
                mShowFullPath = false;
                mCurrentDirectory = fullPathBuffer;
                mNeedRefresh = true;
            }
        }
        
        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 5);
        ImGui::SameLine();
        
        ImGui::Button("Filter");
        
        ImGui::SameLine();
        
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        char searchBuffer[128];
        strncpy_s(searchBuffer, std::string("...").c_str(), sizeof(searchBuffer));
        ImGui::InputText("##Searchbar", searchBuffer, sizeof(searchBuffer));
        
        ImGui::EndChild();
    }
}

bool ContentBrowserPanel::IsSubpathOf(const std::filesystem::path &path,const std::filesystem::path &base)
{
    auto rel = std::filesystem::relative(path, base);
    return !rel.empty() && rel.native()[0] != '.';
}

void ContentBrowserPanel::DrawBreadCrumb(float width)
{
    float startPosX = ImGui::GetCursorPosX();
    auto tex = GetImageFromType(FileType::None, "");
    ImGui::Image(tex, ImVec2(ImGui::GetContentRegionAvail().y, ImGui::GetContentRegionAvail().y));
    for (int i = (static_cast<int>(mPreviousFoldersInHierarchy.size()) - 1); i >= 0; --i)
    {
        ImGui::SameLine();
        std::string folderName = mPreviousFoldersInHierarchy[i].filename().string();
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 50, 50, 255));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(70, 70, 70, 200));
        if (ImGui::Button(folderName.c_str()))
        {
            mCurrentDirectory = mPreviousFoldersInHierarchy[i];
            mNeedRefresh = true;
        }
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        
        if (i != 0)
        {
            ImGui::SameLine();
            ImGui::Text(" > ");
        }
    }
    ImGui::SameLine();
    float newStartPosX = ImGui::GetCursorPosX();
    float remainingWidth = width - newStartPosX + startPosX;
    remainingWidth = std::max<float>(remainingWidth, 50);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::Button("##ShowFullPathBtn", ImVec2(remainingWidth, 0));
    ImGui::PopStyleColor();

    if (ImGui::IsItemClicked())
    {
        mShowFullPath = true;
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
}

void ContentBrowserPanel::ImageButton(bool pIsSelected, const ContentBrowserItem& file, const std::string& name)
{
    ImVec2 size(100, 150);
    ImVec2 imgSize(90, 90);
    ImU32 bgColor;
    ImU32 bgBottomColor;
    float rounding = 8.0f;
    float fontSize = 16.0f;

    ImVec2 start = ImGui::GetCursorScreenPos();
    ImVec2 startImage = ImVec2(start.x + 5, start.y + 5);
    ImVec2 endImage = ImVec2(startImage.x + imgSize.x, startImage.y + imgSize.y);
    ImVec2 end = ImVec2(start.x + size.x, start.y + size.y);
    
    std::string cleanPath = file.mPath.lexically_normal().generic_string();
    auto asset = file.mAsset;
    
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
            break;
        case FileType::Font:
            CreateDragDropSource("FONT", file);
            break;
        case FileType::None:
            break;
        }
    }

    if (ImGui::IsItemActive())
    {
        bgColor = IM_COL32(100, 100, 100, 155);
        bgBottomColor = IM_COL32(100, 100, 100, 255);
    }
    else if (ImGui::IsItemHovered())
    {
        bgColor = IM_COL32(140, 140, 140, 155);
        bgBottomColor = IM_COL32(140, 140, 140, 255);
    }
    else if (pIsSelected)
    {
        bgColor = IM_COL32(0, 0, 0, 0);
        bgBottomColor = IM_COL32(140, 140, 140, 255);
    }
    else
    {
        bgColor = IM_COL32(0, 0, 0, 0);
        bgBottomColor = IM_COL32(60, 60, 60, 255);
    }
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImTextureID myIcon;
    ImVec2 textPos;
    if (asset)
    {
        myIcon = GetImageFromType(asset->mType, asset->mPath);
        textPos = ImVec2(startImage.x, startImage.y + 100);
        ImVec2 rectPos = ImVec2(start.x, startImage.y + 95);
        draw_list->AddRect(start, end, ImColor(60, 60, 60, 255), rounding, 0, 2.0f);
        draw_list->AddRectFilled(rectPos, end, bgBottomColor, 0.0f);
        draw_list->AddLine(rectPos, ImVec2(rectPos.x + 100, rectPos.y), ImColor(230, 179, 0, 255), 3);
        fontSize = 14;

        if (pIsSelected && ImGui::IsItemHovered())
        {
            std::string tooltipText =
                "File : " + file.mPath.filename().string() + "\n" +
                "Path : " + file.mPath.generic_string() + "\n" +
                "Type : " + GetStringFromFileType(asset->mType) + "\n" +
                "File Size : " + StringifyFileSize(std::filesystem::file_size(file.mPath)) + "\n" ;
            ImGui::SetTooltip(tooltipText.c_str());
        }
    }
    else
    {
        myIcon = (ImTextureID)(intptr_t)AssetsManager::GetInstance().LoadTexture("../Content/Sprites/Icons/folder80.png", "../Content/Sprites/Icons/folder80.png")->GetHandle();
        textPos = ImVec2(start.x + 50 - (ImGui::CalcTextSize(name.c_str()).x * 0.5f), startImage.y + 100);
    }

    draw_list->AddRectFilled(start, end, bgColor, rounding);
    draw_list->AddImage(myIcon, startImage, endImage);
    
    draw_list->AddText(ImGui::GetFont(), fontSize, textPos, IM_COL32(255, 255, 255, 255), name.c_str(), 0, 90, 0);
}

void ContentBrowserPanel::CreateDragDropSource(const std::string& name, const ContentBrowserItem& data)
{
    if (!data.mAsset)
    {
        return;
    }
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        ImGui::SetDragDropPayload(name.c_str(), data.mAsset->mId.c_str(), data.mAsset->mId.size());
        ImGui::Text(data.mPath.string().c_str());
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
        std::filesystem::remove(mSelectedEntry.string() + ".meta");
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
            auto id = AssetsManager::GetInstance().GetDatabase().CreateMetaFromFile(newPrefabPath);
            CreateFileAsset(id, newPrefabPath.make_preferred().string());
            mNeedRefresh = true;
        }
        ImGui::EndDragDropTarget();
    }
}

void ContentBrowserPanel::CreateFileAsset(const std::string& id, const std::string& path)
{
    FileAsset asset;
    asset.mId = id;
    asset.mPath = path;
    asset.mExtension = std::filesystem::path(path).extension().string();
    asset.mMetaPath = path + ".meta";
    asset.mType = GetTypeFromExtension(asset.mExtension);
        
    mFileAssets.emplace(path, asset);
}
