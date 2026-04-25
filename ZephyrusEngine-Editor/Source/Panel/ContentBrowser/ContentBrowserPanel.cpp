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
#include "EditorUI/EditorContentIds.h"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

#include <algorithm>

std::filesystem::path ContentBrowserPanel::mRootDirectory = "../Content";
std::filesystem::path ContentBrowserPanel::mCurrentDirectory = mRootDirectory;

using Zephyrus::Assets::AssetsManager;

ContentBrowserPanel::ContentBrowserPanel(ISceneContext* pSceneContext, const std::string& pName, std::shared_ptr<Zephyrus::Editor::Window::WindowManager> pWindowManager)
    : Panel(pSceneContext, pName), mWindowManager(pWindowManager)
{
    auto database = AssetsManager::GetInstance().GetFileDatabase();
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

        bool needOpening = false;
        
        if (ImGui::BeginPopup("CreationPopUP"))
        {
            if (ImGui::BeginMenu("Add"))
            {
                if (ImGui::MenuItem("New Scene"))
                {
                    std::filesystem::path newMapPath = mCurrentDirectory / "NewScene.zpmap";

                    int counter = 1;
                    while (std::filesystem::exists(newMapPath))
                    {
                        newMapPath = mCurrentDirectory / ("NewScene" + std::to_string(counter) + ".zpmap");
                        counter++;
                    }

                    std::ofstream file(newMapPath);
                    if (file.is_open())
                    {
                        file << "{}";
                        file.close();
                    }

                    auto id = AssetsManager::GetInstance().GetFileDatabase().CreateMetaFromFile(newMapPath);
                    CreateFileAsset(id, newMapPath.make_preferred().string());
                    mNeedRefresh = true;
                }
                if (ImGui::MenuItem("New Material"))
                {
                    std::filesystem::path newMaterialPath = mCurrentDirectory / "NewMaterial.zpmat";

                    int counter = 1;
                    while (std::filesystem::exists(newMaterialPath))
                    {
                        newMaterialPath = mCurrentDirectory / ("NewMaterial" + std::to_string(counter) + ".zpmat");
                        counter++;
                    }

                    std::ofstream file(newMaterialPath);
                    if (file.is_open())
                    {
                        file << "{}";
                        file.close();
                    }

                    auto id = AssetsManager::GetInstance().GetFileDatabase().CreateMetaFromFile(newMaterialPath);
                    CreateFileAsset(id, newMaterialPath.make_preferred().string());
                    mNeedRefresh = true;
                }
                ImGui::EndMenu();
            }
            if (!mSelectedEntries.empty())
            {
                if (ImGui::MenuItem("Delete"))
                {
                    DeleteFileOrDirectory();
                }
                if (!mSelectedEntries.empty() && mSelectedEntries[0].mAsset && ImGui::MenuItem("Rename"))
                {
                    needOpening = true;
                }
            }
            ImGui::EndPopup();
        }

        if (!mSelectedEntries.empty() && ImGui::IsKeyPressed(ImGuiKey_Delete) && ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        {
            DeleteFileOrDirectory();
        }
        if (!mSelectedEntries.empty())
        {
            RenameFileAssetPopUp(mSelectedEntries[0], needOpening);
        }

        CreatePrefabFile(mCurrentDirectory.string());

        if (mNeedRefresh)
        {
            RefreshContentBrowser(mCurrentDirectory);
        }
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
    columns = std::max<int>(1, columns);
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

    // TODO : USE TABLES
    ImGui::Columns(columns, 0, false);

    if (mCurrentDirectory != mRootDirectory)
    {
        mIsSelected = !mSelectedEntries.empty() ? (mSelectedEntries[0].mPath == mCurrentDirectory.parent_path()) : false;
        ContentBrowserItem returnFolder;
        returnFolder.mPath = mCurrentDirectory;
        returnFolder.mIsDirectory = true;
        returnFolder.mAsset = nullptr;
        ImageButton(mIsSelected, returnFolder, "...");
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            if (ImGui::IsItemClicked() || (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()))
            {
                mSelectedEntries.clear();
            }
        }

        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
        {
            mCurrentDirectory = mCurrentDirectory.parent_path();
            mSelectedEntries.clear();
            mNeedRefresh = true;
        }

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
    mIsSelected = false;
    if (!mSelectedEntries.empty())
    {
        mIsSelected = mSelectedEntries[0].mPath == item.mPath;
    }

    std::string name = item.mPath.filename().replace_extension("").string();

    ImGui::PushID(name.c_str());

    ImageButton(mIsSelected, item, name);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        if (ImGui::IsItemClicked())
        {
            mSelectedEntries.clear();
            mSelectedEntries.emplace_back(item);
        }
        else if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
        {
            mSelectedEntries.clear();
        }
    }

    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
    {
        if (item.mIsDirectory)
        {
            mCurrentDirectory = item.mPath;
            mSelectedEntries.clear();
            mNeedRefresh = true;
        }
        else // TODO : Create a map and function to open files, if the extension is not found then open with shellexecuteA
        {
            if (item.mAsset->mType != FileType::Material && item.mAsset->mType != FileType::Map)
            {
#ifdef _WIN32
                ShellExecuteA(nullptr, "open", item.mPath.string().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#endif
            }
            else if (item.mAsset->mType == FileType::Map) // load map
            {
                mContext->LoadSceneFromFileId(item.mAsset->mId, nullptr, false);
                mContext->SetSceneLoaded(true);
                mContext->GetRenderer()->GetHud()->Unload();
                mHierarchy->ResetSelectedActor();
                EventSystem::ClearAllEvents();
                resetfunc();
            }
            else if (item.mAsset->mType == FileType::Material)
            {
                std::string fileName = item.mAsset->mFileName;
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

bool ContentBrowserPanel::RenameFileAssetPopUp(const ContentBrowserItem& itemToRename, bool needOpening)
{
    if (!itemToRename.mAsset)
    {
        return false;
    }
    if (needOpening)
    {
        ImGui::OpenPopup("RenameFileAsset");
    }
    
    auto oldPath = itemToRename.mAsset->mPath;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowPos(mRenameAssetButtonStart, ImGuiCond_Always);

    if (ImGui::BeginPopup("RenameFileAsset", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
    {
        const char* renameLabel = "##RenameFileAsset";
		
        char buffer[64];
        strncpy_s(buffer, itemToRename.mAsset->mFileName.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';

        ImGui::SetNextItemWidth(100);
        ImGui::PushStyleColor(ImGuiCol_FrameBg,        IM_COL32(229, 178, 0, 150));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(66, 150, 250, 255));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  IM_COL32(66, 150, 250, 255));
		
        if (ImGui::IsWindowAppearing())
        {
            ImGui::SetKeyboardFocusHere();
        }
		
        if (ImGui::InputText(renameLabel, buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            if (!itemToRename.mAsset->Rename(buffer))
            {
                ImGui::CloseCurrentPopup();
                return false;
            }
            auto newPath = itemToRename.mAsset->mPath;
            auto FileItem = mFileAssets[oldPath];
            mFileAssets.erase(oldPath);
            mFileAssets[newPath] = FileItem;
            mSelectedEntries.clear();
            mNeedRefresh = true;
        }

        if (ImGui::IsItemDeactivated())
        {
            ImGui::CloseCurrentPopup();
        }
		
        ImGui::PopStyleColor(3);
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    return true;
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
        if (!asset)
        {
            bgColor = IM_COL32(100, 100, 100, 255);
        }
        bgBottomColor = IM_COL32(140, 140, 140, 255);
        mRenameAssetButtonStart = ImVec2(start.x, startImage.y + 100);
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
        myIcon = GetImageFromType(asset->mType, asset->mId);
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
        myIcon = (ImTextureID)(intptr_t)AssetsManager::GetInstance().LoadTexture(TEX_FOLDER80_ICON)->GetHandle();
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

ImTextureID ContentBrowserPanel::GetImageFromType(const FileType& type, const std::string& fileId)
{
    Zephyrus::Assets::ITexture2D* tex;

    switch (type)
    {
    case FileType::Image:
        tex = AssetsManager::GetInstance().LoadTexture(fileId);
        break;
    case FileType::Font:
        tex = AssetsManager::GetInstance().LoadTexture(TEX_FONT80_ICON);
        break;
    case FileType::Mesh:
        tex = AssetsManager::GetInstance().LoadTexture(TEX_MESH80_ICON);
        break;
    case FileType::Prefab:
        tex = AssetsManager::GetInstance().LoadTexture(TEX_PREFAB80_ICON);
        break;
    case FileType::Shader:
        tex = AssetsManager::GetInstance().LoadTexture(TEX_SHADER80_ICON);
        break;
    case FileType::Material:
        tex = AssetsManager::GetInstance().LoadTexture(TEX_MAT80_ICON);
        break;
    case FileType::Map:
        tex = AssetsManager::GetInstance().LoadTexture(TEX_SCENE80_ICON);
        break;
    default:
        tex = AssetsManager::GetInstance().LoadTexture(TEX_FOLDER80_ICON);
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
    if (mSelectedEntries.empty())
    {
        return;
    }
    for (auto& entry : mSelectedEntries)
    {
        if (std::filesystem::is_directory(entry.mPath))
        {
            std::filesystem::remove_all(entry.mPath);
        }
        else
        {
            std::filesystem::remove(entry.mPath);
            std::filesystem::remove(entry.mPath.string() + ".meta");
        }
    }
    mSelectedEntries.clear();
    mNeedRefresh = true;
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
            auto id = AssetsManager::GetInstance().GetFileDatabase().CreateMetaFromFile(newPrefabPath);
            CreateFileAsset(id, newPrefabPath.make_preferred().string());
            mNeedRefresh = true;
        }
        ImGui::EndDragDropTarget();
    }
}

void ContentBrowserPanel::CreateFileAsset(const std::string& id, const std::string& path)
{
    auto fsPath = std::filesystem::path(path);
    FileAsset asset;
    asset.mId = id;
    asset.mPath = path;
    asset.mFileName = fsPath.stem().string();
    asset.mExtension = fsPath.extension().string();
    asset.mMetaPath = path + ".meta";
    asset.mType = GetTypeFromExtension(asset.mExtension);
        
    mFileAssets.emplace(path, asset);
}
