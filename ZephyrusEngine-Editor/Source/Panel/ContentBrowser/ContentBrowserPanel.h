#pragma once
#include "../Panel.h"
#include <filesystem>
#include "../SceneHierarchyPanel.h"
#include <functional>

#include "Window/EditorWindow.h"

namespace Zephyrus::Editor::Window
{
	class WindowManager;
}

class ContentBrowserPanel : public Panel
{
private:
	static std::filesystem::path rootDirectory;
	static std::filesystem::path currentDirectory;
	SceneHierarchyPanel* mHierarchy{ nullptr };
	using ResetEditorController = std::function<void()>;
	std::shared_ptr<Zephyrus::Editor::Window::WindowManager> mWindowManager{ nullptr };
public:
	ContentBrowserPanel(ISceneContext* pSceneContext, const std::string& pName, std::shared_ptr<Zephyrus::Editor::Window::WindowManager> pWindowManager);
	~ContentBrowserPanel() override = default;
	void Draw() override;
	void DrawDirectory(const std::string& folderPath);
	void DrawDirectoryContent(const std::filesystem::path& directory);
	void DrawEntry(const std::filesystem::directory_entry& entry);
	void ImageButton(bool pIsSelected, const std::string& entry, const std::string& extension);
	ImTextureID GetImageFromExtension(const std::string& extension, std::string filepath = "");
	void SetSceneHierarchy(SceneHierarchyPanel* pHierarchy);
	void DeleteFileOrDirectory();
	void CreatePrefabFile(const std::string& pFilepath);
	
	ResetEditorController resetfunc;
};
