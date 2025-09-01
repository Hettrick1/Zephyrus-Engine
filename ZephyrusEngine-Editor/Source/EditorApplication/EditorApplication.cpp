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

const std::string consolePanelName = "Console";
const std::string inspectorPanelName = "Inspector";
const std::string scenePanelName = "Scene";
const std::string sceneHierarchyName = "Scene Hierarchy";
const std::string contentBrowserName = "Content Browser";

EditorApplication::EditorApplication(const std::string& pTitle, Scene* pStartupScene)
    : mIsRunning(true), mStartUpScene(pStartupScene), mInputManager(InputManager::Instance()), mPhysicManager(PhysicManager::Instance())
    , mCameraManager(CameraManager::Instance()), mTitle(pTitle)
{
    Zephyrus::Log::Init();
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
    }
    else
    {
        ZP_CORE_INFO("SDL initialization succeeded!");
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
    SDL_DisplayMode displayMode;

    mGameWindow = new Window(1920, 1030, true);
    mRenderer = new RendererOpenGl();
    if (mGameWindow->Open(mTitle) && mRenderer->Initialize(*mGameWindow) && TextRenderer::Instance().Init(*mGameWindow)) 
    {
        SDL_MaximizeWindow(mGameWindow->GetSdlWindow());

        InitializeFrameBuffer();

        // Init ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); 
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL(mGameWindow->GetSdlWindow(), SDL_GL_GetCurrentContext());
        ImGui_ImplOpenGL3_Init("#version 450");

        InitializePanels();

        SceneManager::LoadScene(mStartUpScene, false);
        SceneManager::mIsSceneLoaded = true;
        Loop();
    }
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

void EditorApplication::Loop()
{
    SceneManager::StartScene(mRenderer);
    mRenderer->SetHud(nullptr);

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

    SetEditorStyle();

    DrawDockSpace();
    DrawPanels();

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

void EditorApplication::Input()
{
    if (mIsRunning) {
        while (SDL_PollEvent(&mSdlEvent)) {
            ImGui_ImplSDL2_ProcessEvent(&mSdlEvent);
            switch (mSdlEvent.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
            }
        }
        mInputManager.Update();
    }
}

void EditorApplication::InitializePanels()
{
    std::unique_ptr<ConsolePanel> consolePanel = std::make_unique<ConsolePanel>(consolePanelName);
    std::unique_ptr<ScenePanel> scenePanel = std::make_unique<ScenePanel>(scenePanelName, mRenderTexture);
    std::unique_ptr<InspectorPanel> inspectorPanel = std::make_unique<InspectorPanel>(inspectorPanelName);
    std::unique_ptr<SceneHierarchyPanel> sceneHierarchyPanel = std::make_unique<SceneHierarchyPanel>(sceneHierarchyName);
    std::unique_ptr<ContentBrowserPanel> contentBrowserPanel = std::make_unique<ContentBrowserPanel>(contentBrowserName);

    ConsolePanel* consolePanelRaw = consolePanel.get();

    mAllPanels[inspectorPanelName] = std::move(inspectorPanel);
    mAllPanels[sceneHierarchyName] = std::move(sceneHierarchyPanel);
    mAllPanels[consolePanelName] = std::move(consolePanel);
    mAllPanels[contentBrowserName] = std::move(contentBrowserPanel);
    mAllPanels[scenePanelName] = std::move(scenePanel);

    Zephyrus::Log::AddListener(consolePanelRaw);
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
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
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

        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_id_right;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, &dock_id_right, &dock_main_id);

        ImGuiID dock_id_down;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, &dock_id_down, &dock_main_id);

        ImGuiID dock_id_left;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.25f, &dock_id_left, &dock_main_id);

        ImGui::DockBuilderDockWindow(scenePanelName.c_str(), dock_main_id);
        ImGui::DockBuilderDockWindow(consolePanelName.c_str(), dock_id_down);
        ImGui::DockBuilderDockWindow(contentBrowserName.c_str(), dock_id_down);
        ImGui::DockBuilderDockWindow(inspectorPanelName.c_str(), dock_id_right);
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
    colors[ImGuiCol_Border] = ImVec4(1.0f, 0.81176f, 0.0f, 1.0f);

    colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.81176f, 0.0f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

    colors[ImGuiCol_TabSelected] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    colors[ImGuiCol_Tab] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    colors[ImGuiCol_DockingPreview] = ImVec4(1.0f, 0.81176f, 0.0f, 0.8f);

    colors[ImGuiCol_Button] = ImVec4(0.2f, 0.3f, 0.7f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.4f, 0.8f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.2f, 0.5f, 1.0f);
}

void EditorApplication::Close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    mEditorController = nullptr;
    mAllPanels.clear();
    SceneManager::Unload();
    mGameWindow->Close();
    Zephyrus::Log::Shutdown();
}
