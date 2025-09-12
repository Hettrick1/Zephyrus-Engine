#include "EditorApplication.h"
#include "Log.h"
#include "RendererOpenGl.h"
#include "TextRenderer.h"
#include "SplashScreen.h"
#include "PrefabFactory.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "Panel/InspectorPanel.h"
#include "Panel/ScenePanel.h"
#include "Panel/ConsolePanel.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/ContentBrowserPanel.h"
#include "EditorControllerActor.h"
#include "EditorControllerComponent.h"
#include "Panel/MenuPanel.h"
#include "Panel/UtilsPanel.h"
#include "Panel/PrefabPanel.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "HudManager.h"

EditorApplication::EditorApplication(const std::string& pTitle, const std::string& pStartupScene)
    : mIsRunning(true), mStartUpScene(pStartupScene), mInputManager(InputManager::Instance()), mPhysicManager(PhysicManager::Instance())
    , mCameraManager(CameraManager::Instance()), mTitle(pTitle)
{
    Zephyrus::Log::Init();
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        ZP_EDITOR_ERROR(std::string("SDL init failed: ") + SDL_GetError());
    }
    else
    {
        ZP_EDITOR_INFO("SDL initialization succeeded!");
    }

    Initialize();
}

EditorApplication::~EditorApplication()
{
    delete mRenderer;
    delete mGameWindow;
}

void EditorApplication::Initialize()
{
    mGameWindow = new Window(1920, 1030, true);
    mRenderer = new RendererOpenGl();
    if (mGameWindow->Open(mTitle) && mRenderer->Initialize(*mGameWindow) && TextRenderer::Instance().Init(*mGameWindow)) 
    {
        SDL_MaximizeWindow(mGameWindow->GetSdlWindow());

        SDL_Surface* icon = IMG_Load("../Content/Sprites/Icons/ZephyrusLogo.png");
        SDL_SetWindowIcon(mGameWindow->GetSdlWindow(), icon);
        SDL_FreeSurface(icon);

        InitializeImGui();

        InitializeFrameBuffer();

        InitializePanels();

        Loop();
    }
}

void EditorApplication::InitializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ZP::UI::gFonts.small = io.Fonts->AddFontFromFileTTF("../Content/Fonts/Roboto/static/Roboto-SemiBold.ttf", 16.0f);
    ZP::UI::gFonts.medium = io.Fonts->AddFontFromFileTTF("../Content/Fonts/Roboto/static/Roboto-SemiBold.ttf", 24.0f);
    ZP::UI::gFonts.large = io.Fonts->AddFontFromFileTTF("../Content/Fonts/Roboto/static/Roboto-SemiBold.ttf", 32.0f);

    if (!ZP::UI::gFonts.small || !ZP::UI::gFonts.medium || !ZP::UI::gFonts.large)
    {
        ZP_EDITOR_ERROR("Font not loaded !");
    }

    ImGui_ImplSDL2_InitForOpenGL(mGameWindow->GetSdlWindow(), SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init("#version 450");
}

void EditorApplication::InitializeFrameBuffer()
{
    glGenFramebuffers(1, &mFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

    glGenTextures(1, &mRenderTexture);

    glBindTexture(GL_TEXTURE_2D, mRenderTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &mDepthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderBuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderTexture, 0);

    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ZP_EDITOR_ERROR("Frame buffer init failed !");
    }
}

void EditorApplication::InitializePanels()
{
    std::unique_ptr<PrefabPanel> prefabPanel = std::make_unique<PrefabPanel>(prefabPanelName);
    std::unique_ptr<ConsolePanel> consolePanel = std::make_unique<ConsolePanel>(consolePanelName);
    std::unique_ptr<ScenePanel> scenePanel = std::make_unique<ScenePanel>(scenePanelName, mRenderTexture);
    std::unique_ptr<InspectorPanel> inspectorPanel = std::make_unique<InspectorPanel>(inspectorPanelName);
    std::unique_ptr<SceneHierarchyPanel> sceneHierarchyPanel = std::make_unique<SceneHierarchyPanel>(sceneHierarchyName);
    std::unique_ptr<ContentBrowserPanel> contentBrowserPanel = std::make_unique<ContentBrowserPanel>(contentBrowserName);
    std::unique_ptr<MenuPanel> menuPanel = std::make_unique<MenuPanel>(menuPanelName, this);
    std::unique_ptr<UtilsPanel> utilsPanel = std::make_unique<UtilsPanel>(utilsPanelName, topBarHeight);

    ConsolePanel* consolePanelRaw = consolePanel.get();
    SceneHierarchyPanel* hierarchyPanelRaw = sceneHierarchyPanel.get();

    inspectorPanel->SetSceneHierarchy(hierarchyPanelRaw);
    contentBrowserPanel->SetSceneHierarchy(hierarchyPanelRaw);
    contentBrowserPanel->resetfunc = [this]() { this->ResetEditorController(); };

    mAllPanels[inspectorPanelName] = std::move(inspectorPanel);
    mAllPanels[prefabPanelName] = std::move(prefabPanel);
    mAllPanels[sceneHierarchyName] = std::move(sceneHierarchyPanel);
    mAllPanels[consolePanelName] = std::move(consolePanel);
    mAllPanels[contentBrowserName] = std::move(contentBrowserPanel);
    mAllPanels[scenePanelName] = std::move(scenePanel);
    mAllPanels[utilsPanelName] = std::move(utilsPanel);
    mAllPanels[menuPanelName] = std::move(menuPanel);

    Zephyrus::Log::AddListener(consolePanelRaw);
}

void EditorApplication::Loop()
{
    SceneManager::LoadSceneWithFile(mStartUpScene, mRenderer, false);
    SceneManager::mIsSceneLoaded = true;

    mRenderer->GetHud()->Unload();

    SceneManager::ActiveScene->GetRenderer()->GetDebugRenderer()->SetDrawSelected(true);

    auto editorController = new EditorControllerActor();
    mEditorController = editorController;

    while (mIsRunning) {
        Timer::ComputeDeltaTime();
        Input();
        Update();
        Render();
        Timer::DelayTime();
    }

    Close();
}

void EditorApplication::Update()
{
    mCameraManager.UpdateCurrentCamera();
    for (auto& panel : mAllPanels)
    {
        panel.second->Update();
    }
    auto it = mAllPanels.find(scenePanelName);
    if (it != mAllPanels.end())
    {
        if (auto scenePanel = dynamic_cast<ScenePanel*>(it->second.get()))
        {
            mEditorController->GetComponentOfType<EditorControllerComponent>()->SetIsInSceneCapture(scenePanel->GetIsHover());
        }
    }
}

void EditorApplication::Render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

    SceneManager::BeginRender();
    SceneManager::RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, mGameWindow->GetDimensions().x, mGameWindow->GetDimensions().y);

    RenderImgui();
    SceneManager::EndRender();
}

void EditorApplication::RenderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();

    bool isFpsMode = mEditorController->GetComponentOfType<EditorControllerComponent>()->GetCanMove();

    io.WantCaptureMouse = !isFpsMode;
    io.MouseDrawCursor = !isFpsMode;

    SetEditorStyle();

    DrawDockSpace();
    DrawPanels();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

void EditorApplication::Input()
{
    if (mIsRunning) {
        while (SDL_PollEvent(&mSdlEvent)) {
            ImGui_ImplSDL2_ProcessEvent(&mSdlEvent);
            if (mSdlEvent.type == SDL_WINDOWEVENT && mSdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                if (mSdlEvent.window.windowID == SDL_GetWindowID(mGameWindow->GetSdlWindow()))
                {
                    mIsRunning = false;
                }
            }
            if (mSdlEvent.type == SDL_KEYDOWN) 
            {
                const Uint8* state = SDL_GetKeyboardState(nullptr);
                bool ctrl = state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL];

                if (ctrl && mSdlEvent.key.keysym.sym == SDLK_z) 
                {
                    EventSystem::UndoLastEvent();
                }
            }
        }
        mInputManager.Update();
    }
}

void EditorApplication::DrawDockSpace()
{
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = 
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
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

void EditorApplication::DrawPanels()
{
    for (auto& panel : mAllPanels)
    {
        panel.second->Draw();
    }
}

void EditorApplication::SetEditorStyle()
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

    colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.0f);
}

void EditorApplication::ResetEditorController()
{
    delete mEditorController;
    mEditorController = new EditorControllerActor();
}

void EditorApplication::Close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    Zephyrus::Log::Shutdown();
    mAllPanels.clear();
    mEditorController->Destroy();
    delete mEditorController;
    SceneManager::Unload();
    mGameWindow->Close();
    EventSystem::ClearAllEvents();
}

Panel* EditorApplication::GetPanelWithName(std::string pPanelName)
{
    auto it = mAllPanels.find(pPanelName);
    if (it != mAllPanels.end())
    {
        return it->second.get();
    }
}
