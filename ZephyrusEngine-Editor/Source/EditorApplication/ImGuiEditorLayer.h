#pragma once

#include "Panel/Panel.h"
#include <iostream>
#include "../Window/WindowManager.h"
#include <unordered_map>
#include <memory>
#include <imgui_internal.h>

class EditorApplication;

class ImGuiEditorLayer
{
private:
	std::unordered_map<std::string, std::unique_ptr<Panel>> mAllPanels;
	std::shared_ptr<Zephyrus::Editor::Window::WindowManager> mWindowManager {nullptr};
	ImGuiContext* mEditorContext{ nullptr };
public:
	ImGuiEditorLayer() = default;
	~ImGuiEditorLayer() = default;

	void UpdatePanels(EditorApplication* editor);
	
	void InitializeImGui(SDL_Window* mSdlWindow);
	void InitializePanels(EditorApplication* editor);

	void RenderImgui();

	void DrawDockSpace();
	void DrawPanels();

	void SetEditorStyle();

	void CloseImGui();

public:
	Panel* GetPanelWithName(std::string pPanelName);

public:
	const std::string consolePanelName = "Console";
	const std::string inspectorPanelName = "Inspector";
	const std::string scenePanelName = "Scene";
	const std::string sceneHierarchyName = "Scene Hierarchy";
	const std::string contentBrowserName = "Content Browser";
	const std::string menuPanelName = "MenuPanel";
	const std::string utilsPanelName = "UtilsPanel";
	const std::string prefabPanelName = "Prefabs";
	const float topBarHeight = 45.0f;
};
