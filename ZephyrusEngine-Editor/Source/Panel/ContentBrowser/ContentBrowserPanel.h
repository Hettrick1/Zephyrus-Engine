#pragma once
#include "../Panel.h"
#include <filesystem>
#include "../SceneHierarchyPanel.h"
#include <functional>

#include "Window/EditorWindow.h"

enum class FileType;
struct FileAsset;

namespace Zephyrus::Editor::Window
{
	class WindowManager;
}

class ContentBrowserPanel : public Panel
{
private:
	struct ContentBrowserItem {
		bool mIsDirectory { true };
		std::filesystem::path mPath;
		FileAsset* mAsset { nullptr };
	};
	
	static std::filesystem::path mRootDirectory;
	static std::filesystem::path mCurrentDirectory;
	SceneHierarchyPanel* mHierarchy{ nullptr };
	using ResetEditorController = std::function<void()>;
	std::shared_ptr<Zephyrus::Editor::Window::WindowManager> mWindowManager{ nullptr };

	std::unordered_map<std::string, FileAsset> mFileAssets;
	std::vector<ContentBrowserItem> mCurrentItemsInFolder;

	bool mIsSelected{ false };
	bool mNeedRefresh{ false };
	std::vector<ContentBrowserItem> mSelectedEntries{};
	bool mShowFullPath{ false };
	std::vector<std::filesystem::path> mPreviousFoldersInHierarchy;
	ImVec2 mRenameAssetButtonStart {0.0f, 0.0f};

	void ImageButton(bool pIsSelected, const ContentBrowserItem& file, const std::string& name);
public:
	ContentBrowserPanel(ISceneContext* pSceneContext, const std::string& pName, std::shared_ptr<Zephyrus::Editor::Window::WindowManager> pWindowManager);
	~ContentBrowserPanel() override = default;

	void RefreshContentBrowser(std::filesystem::path& path);
	
	void Draw() override;
	void DrawDirectoryTree(const std::string& folderPath);
	void DrawDirectoryContent();
	void DrawItem(ContentBrowserItem& item);
	void DrawBrowserUtils(float width);
	void DrawBreadCrumb(float width);
	bool IsSubpathOf(const std::filesystem::path &path,const std::filesystem::path &base);

	bool RenameFileAssetPopUp(const ContentBrowserItem& itemToRename, bool needOpening);

	void CreateDragDropSource(const std::string& name, const ContentBrowserItem&  data);
	ImTextureID GetImageFromType(const FileType& type, const std::string& fileId);
	void SetSceneHierarchy(SceneHierarchyPanel* pHierarchy);
	void DeleteFileOrDirectory();
	void CreatePrefabFile(const std::string& pFilepath);

	void CreateFileAsset(const std::string& id, const std::string& path);
	
	ResetEditorController resetfunc;
};
