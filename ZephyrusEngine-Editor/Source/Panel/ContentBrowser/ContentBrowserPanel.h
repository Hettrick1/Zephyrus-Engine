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
		bool isDirectory { true };
		std::filesystem::path path;
		FileAsset* asset { nullptr };
	};
	
	static std::filesystem::path mRootDirectory;
	static std::filesystem::path mCurrentDirectory;
	SceneHierarchyPanel* mHierarchy{ nullptr };
	using ResetEditorController = std::function<void()>;
	std::shared_ptr<Zephyrus::Editor::Window::WindowManager> mWindowManager{ nullptr };

	std::unordered_map<std::string, FileAsset> mFileAssets;
	std::vector<ContentBrowserItem> mCurrentItemsInFolder;

	bool mIsSelected{ false };
	std::filesystem::path mSelectedEntry;
public:
	ContentBrowserPanel(ISceneContext* pSceneContext, const std::string& pName, std::shared_ptr<Zephyrus::Editor::Window::WindowManager> pWindowManager);
	~ContentBrowserPanel() override = default;

	void RefreshContentBrowser(std::filesystem::path& path);
	
	void Draw() override;
	void DrawDirectory(const std::string& folderPath);
	void DrawDirectoryContent();
	void DrawItem(ContentBrowserItem& item);
	void ImageButton(bool pIsSelected, const ContentBrowserItem& file);
	void CreateDragDropSource(const std::string& name, const ContentBrowserItem&  data);
	ImTextureID GetImageFromType(const FileType& type, const std::string& filepath);
	void SetSceneHierarchy(SceneHierarchyPanel* pHierarchy);
	void DeleteFileOrDirectory();
	void CreatePrefabFile(const std::string& pFilepath);
	
	ResetEditorController resetfunc;
};
