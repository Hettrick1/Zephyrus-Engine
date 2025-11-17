#include "ImGuiEditorLayer.h"
#include "Log.h"
#include "PrefabFactory.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "Panel/InspectorPanel/InspectorPanel.h"
#include "Panel/ScenePanel.h"
#include "Panel/ConsolePanel.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/ContentBrowserPanel.h"
#include "Panel/MenuPanel.h"
#include "Panel/UtilsPanel.h"
#include "Panel/PrefabPanel.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "HudManager.h"
#include "SceneManager.h"
#include "EditorApplication.h"
#include "EditorControllerActor.h"
#include "EditorControllerComponent.h"

void ImGuiEditorLayer::InitializeImGui(SDL_Window* mSdlWindow)
{
    IMGUI_CHECKVERSION();
    mEditorContext = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ZP::UI::gFonts.small = io.Fonts->AddFontFromFileTTF("../Content/Fonts/Roboto/static/Roboto-SemiBold.ttf", 16.0f);
    ZP::UI::gFonts.medium = io.Fonts->AddFontFromFileTTF("../Content/Fonts/Roboto/static/Roboto-SemiBold.ttf", 20.0f);
    ZP::UI::gFonts.large = io.Fonts->AddFontFromFileTTF("../Content/Fonts/Roboto/static/Roboto-SemiBold.ttf", 28.0f);

    if (!ZP::UI::gFonts.small || !ZP::UI::gFonts.medium || !ZP::UI::gFonts.large)
    {
        ZP_EDITOR_ERROR("Font not loaded !");
    }

    ImGui_ImplSDL2_InitForOpenGL(mSdlWindow, SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init("#version 450");

    mWindowManager = std::make_shared<Zephyrus::Editor::Window::WindowManager>();
}

void ImGuiEditorLayer::InitializePanels(EditorApplication* editor)
{
    std::unique_ptr<PrefabPanel> prefabPanel = std::make_unique<PrefabPanel>(editor->GetSceneManager(), prefabPanelName);
    std::unique_ptr<ConsolePanel> consolePanel = std::make_unique<ConsolePanel>(editor->GetSceneManager(), consolePanelName);
    std::unique_ptr<ScenePanel> scenePanel = std::make_unique<ScenePanel>(editor->GetSceneManager(), scenePanelName, editor->GetEditorController()->GetComponentOfType<Zephyrus::ActorComponent::CameraComponent>()->GetRenderTarget()->GetColorTexture());
    std::unique_ptr<InspectorPanel> inspectorPanel = std::make_unique<InspectorPanel>(editor->GetSceneManager(), inspectorPanelName);
    std::unique_ptr<SceneHierarchyPanel> sceneHierarchyPanel = std::make_unique<SceneHierarchyPanel>(editor->GetSceneManager(), sceneHierarchyName);
    std::unique_ptr<ContentBrowserPanel> contentBrowserPanel = std::make_unique<ContentBrowserPanel>(editor->GetSceneManager(), contentBrowserName, mWindowManager);
    std::unique_ptr<MenuPanel> menuPanel = std::make_unique<MenuPanel>(editor->GetSceneManager(), menuPanelName, this);
    std::unique_ptr<UtilsPanel> utilsPanel = std::make_unique<UtilsPanel>(editor->GetSceneManager(), utilsPanelName, topBarHeight);

    ConsolePanel* consolePanelRaw = consolePanel.get();
    SceneHierarchyPanel* hierarchyPanelRaw = sceneHierarchyPanel.get();

    inspectorPanel->SetSceneHierarchy(hierarchyPanelRaw);
    contentBrowserPanel->SetSceneHierarchy(hierarchyPanelRaw);
    contentBrowserPanel->resetfunc = [this, editor]()
    {
        auto it = mAllPanels.find(inspectorPanelName);
        if (it != mAllPanels.end())
        {
            if (auto inspectorPanel = dynamic_cast<InspectorPanel*>(it->second.get()))
            {
                inspectorPanel->ResetActiveComponent();
            }
        }
        editor->ResetEditorController();
    };

    mAllPanels[inspectorPanelName] = std::move(inspectorPanel);
    mAllPanels[prefabPanelName] = std::move(prefabPanel);
    mAllPanels[sceneHierarchyName] = std::move(sceneHierarchyPanel);
    mAllPanels[consolePanelName] = std::move(consolePanel);
    mAllPanels[contentBrowserName] = std::move(contentBrowserPanel);
    mAllPanels[scenePanelName] = std::move(scenePanel);
    mAllPanels[utilsPanelName] = std::move(utilsPanel);
    mAllPanels[menuPanelName] = std::move(menuPanel);

    Zephyrus::Debug::Log::AddListener(consolePanelRaw);
}

void ImGuiEditorLayer::UpdatePanels(EditorApplication* editor)
{
    auto inspector = mAllPanels.find(inspectorPanelName);
    if (inspector != mAllPanels.end())
    {
        if (auto inspectorPanel = dynamic_cast<InspectorPanel*>(inspector->second.get()))
        {
            auto cam = inspectorPanel->GetCurrentCameraComponent();
            if (cam)
            {
                cam->UpdateMatrices();
                cam->RenderScene();
            }
        }
    }
    auto scene = mAllPanels.find(scenePanelName);
    if (scene != mAllPanels.end())
    {
        if (auto scenePanel = dynamic_cast<ScenePanel*>(scene->second.get()))
        {
            editor->GetEditorController()->GetComponentOfType<Zephyrus::ActorComponent::CameraComponent>()->SetDimensions(scenePanel->GetDimensions());
        }
    }
    for (auto& panel : mAllPanels)
    {
        panel.second->Update();
    }
    auto it = mAllPanels.find(scenePanelName);
    if (it != mAllPanels.end())
    {
        if (auto scenePanel = dynamic_cast<ScenePanel*>(it->second.get()))
        {
            editor->GetEditorController()->GetComponentOfType<Zephyrus::ActorComponent::EditorControllerComponent>()->SetIsInSceneCapture(scenePanel->GetIsHover());
        }
    }
}

void ImGuiEditorLayer::RenderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    SetEditorStyle();
    
    DrawDockSpace();
    DrawPanels();
    mWindowManager->DrawWindows();
    
    auto it = mAllPanels.find(inspectorPanelName);
    if (it != mAllPanels.end())
    {
        if (auto inspectorPanel = dynamic_cast<InspectorPanel*>(it->second.get()))
        {
            auto cam = inspectorPanel->GetCurrentCameraComponent();
            if (cam)
            {
                if (ImGui::Begin("Camera Preview")) 
                { 
                    ImVec2 previewSize = ImGui::GetContentRegionAvail();
                    ImGui::Image((ImTextureID)(intptr_t)cam->GetRenderTarget()->GetColorTexture(), previewSize, ImVec2(0, 1), ImVec2(1, 0));
                } 
                ImGui::End();
            }
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

void ImGuiEditorLayer::DrawDockSpace()
{
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = 
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoNavFocus;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + topBarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - topBarHeight));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("MainDockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspaceFlags);
    ImGui::End();

    static bool first_time = true;
    if (first_time)
    {
        first_time = false;

        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        // creates split dockspaces
        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_id_right;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, &dock_id_right, &dock_main_id);
        ImGuiID dock_id_down;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, &dock_id_down, &dock_main_id);
        ImGuiID dock_id_up;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.1f, &dock_id_up, &dock_main_id);
        ImGuiID dock_id_left;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.25f, &dock_id_left, &dock_main_id);

        ImGui::DockBuilderDockWindow(scenePanelName.c_str(), dock_main_id);
        ImGui::DockBuilderDockWindow(consolePanelName.c_str(), dock_id_down);
        ImGui::DockBuilderDockWindow(contentBrowserName.c_str(), dock_id_down);
        ImGui::DockBuilderDockWindow(inspectorPanelName.c_str(), dock_id_right);
        ImGui::DockBuilderDockWindow(prefabPanelName.c_str(), dock_id_right);
        ImGui::DockBuilderDockWindow(sceneHierarchyName.c_str(), dock_id_left);

        ImGui::DockBuilderFinish(dockspace_id);
    }
}

void ImGuiEditorLayer::DrawPanels()
{
    for (auto& panel : mAllPanels)
    {
        panel.second->Draw();
    }
}

void ImGuiEditorLayer::SetEditorStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    colors[ImGuiCol_Border] = ImVec4(0.9f, 0.7f, 0.0f, 1.0f);

    colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.9f, 0.7f, 0.0f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

    colors[ImGuiCol_TabSelected] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    colors[ImGuiCol_Tab] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    colors[ImGuiCol_DockingPreview] = ImVec4(1.0f, 0.81176f, 0.0f, 0.8f);

    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.0f);
}

Panel* ImGuiEditorLayer::GetPanelWithName(std::string pPanelName)
{
    auto it = mAllPanels.find(pPanelName);
    if (it != mAllPanels.end())
    {
        return it->second.get();
    }
    return nullptr;
}

void ImGuiEditorLayer::CloseImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(mEditorContext);
    mAllPanels.clear();
}