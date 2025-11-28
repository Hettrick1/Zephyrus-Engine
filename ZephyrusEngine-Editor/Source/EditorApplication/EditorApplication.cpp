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
#include "Panel/InspectorPanel/InspectorPanel.h"
#include "Panel/ScenePanel.h"
#include "Panel/ConsolePanel.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/ContentBrowser/ContentBrowserPanel.h"
#include "EditorControllerActor.h"
#include "EditorControllerComponent.h"
#include "Panel/MenuPanel.h"
#include "Panel/UtilsPanel.h"
#include "Panel/PrefabPanel.h"
#include "EditorUI/ImGuiUtils.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "HudManager.h"

EditorApplication::EditorApplication(const std::string& pTitle, const std::string& pStartupScene)
    : mIsRunning(true), mStartUpScene(pStartupScene), mInputManager(InputManager::Instance()), mTitle(pTitle)
{
    Zephyrus::Debug::Log::Init();
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
    delete mEditorController;
    delete mGameWindow;
    delete mSceneManager;
}

void EditorApplication::Initialize()
{
    mGameWindow = new Window(1920, 1030, true);
    mRenderer = new Zephyrus::Render::RendererOpenGl();
    mSceneManager = new Zephyrus::Scenes::SceneManager(mRenderer);
    Zephyrus::Assets::AssetsManager::SetContext(mSceneManager);

    mImGuiEditorLayer = std::make_unique<ImGuiEditorLayer>();
    
    // For now
    InputManager::Instance().SetContext(mSceneManager);

    if (mGameWindow->Open(mTitle) && mRenderer->Initialize(*mGameWindow) && Zephyrus::Render::TextRenderer::Instance().Init(*mGameWindow))
    {
        SDL_MaximizeWindow(mGameWindow->GetSdlWindow());

        SDL_Surface* icon = IMG_Load("../Content/Sprites/Icons/ZephyrusLogo.png");
        SDL_SetWindowIcon(mGameWindow->GetSdlWindow(), icon);
        SDL_FreeSurface(icon);

        auto editorController = new Zephyrus::ActorComponent::EditorControllerActor(mSceneManager, *mSceneManager->GetActiveScene());
        mEditorController = editorController;
        mEditorController->Start();

        mImGuiEditorLayer->InitializeImGui(mGameWindow->GetSdlWindow());
        mImGuiEditorLayer->InitializePanels(this);

        Loop();
    }
}

void EditorApplication::Loop()
{
    mSceneManager->LoadSceneWithFile(mStartUpScene, mRenderer, false);
    mSceneManager->SetSceneLoaded(true);

    mRenderer->GetHud()->Unload();

    mSceneManager->GetActiveScene()->GetRenderer()->GetDebugRenderer()->SetDrawSelected(true);
    
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
    mEditorController->GetComponentOfType<Zephyrus::ActorComponent::CameraComponent>()->UpdateMatrices();
    mImGuiEditorLayer->UpdatePanels(this);
    auto world = mSceneManager->GetPhysicsWorld();
    world->Update(0);

    //EventSystem::Update();
}

void EditorApplication::Render()
{
    mEditorController->GetComponentOfType<Zephyrus::ActorComponent::CameraComponent>()->RenderScene();
    mSceneManager->GetActiveScene()->EndRender();

    mImGuiEditorLayer->RenderImgui();
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

void EditorApplication::ResetEditorController()
{
    mEditorController->Start();
}

void EditorApplication::Close()
{
    mImGuiEditorLayer->CloseImGui();
    Zephyrus::Debug::Log::Shutdown();
    mEditorController->Destroy();
    mSceneManager->Unload();
    mGameWindow->Close();
    EventSystem::ClearAllEvents();
}

