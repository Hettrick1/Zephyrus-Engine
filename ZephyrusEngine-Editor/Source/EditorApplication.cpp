#include "EditorApplication.h"
#include "Log.h"
#include "RendererOpenGl.h"
#include "TextRenderer.h"
#include "SplashScreen.h"
#include "PrefabFactory.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

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
    SDL_GetCurrentDisplayMode(0, &displayMode);
    int screenWidth = displayMode.w;
    int screenHeight = displayMode.h;
    mGameWindow = new Window(screenWidth, screenHeight, true);
    mRenderer = new RendererOpenGl();
    if (mGameWindow->Open(mTitle) && mRenderer->Initialize(*mGameWindow) && TextRenderer::Instance().Init(*mGameWindow)) 
    {
        SDL_MaximizeWindow(mGameWindow->GetSdlWindow());

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

        SceneManager::LoadScene(mStartUpScene);
        Loop();
    }
}

void EditorApplication::Loop()
{
    SceneManager::StartScene(mRenderer);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    mRenderer->SetHud(nullptr);

    PrefabFactory::CreateActorFromPrefab("CameraActor");

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
}

void EditorApplication::Render()
{
    SceneManager::BeginRender();
    SceneManager::RenderScene();
    RenderImgui();
    SceneManager::EndRender();
}

void EditorApplication::RenderImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, world!");
    ImGui::Text("ImGui marche !");
    ImGui::End();

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
            case SDL_KEYDOWN:
                if (mSdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    mIsRunning = false;
                    break;
                }
            }
        }
        mInputManager.Update();
    }
}

void EditorApplication::Close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SceneManager::Unload();
    mGameWindow->Close();
    Zephyrus::Log::Shutdown();
}
