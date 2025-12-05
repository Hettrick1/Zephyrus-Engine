#include "EditorApplication.h"
#include "Log.h"
#include "RendererOpenGl.h"
#include "TextRenderer.h"
#include "SplashScreen.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Panel/InspectorPanel/InspectorPanel.h"
#include "Panel/ContentBrowser/ContentBrowserPanel.h"
#include "EditorControllerActor.h"
#include "EditorControllerComponent.h"
#include "EditorApplication/EventSystem/EventSystem.h"
#include "HudManager.h"

EditorApplication::EditorApplication(const std::string& pTitle, const std::string& pStartupScene)
    : mIsRunning(true), mStartUpScene(pStartupScene), mEditorInputManager(nullptr), mTitle(pTitle)
{
    Zephyrus::Debug::Log::Init();
    Initialize();
}

EditorApplication::~EditorApplication()
{
    delete mRenderer;
    delete mEditorController;
    delete mEditorWindow;
    delete mSceneManager;
    delete mEditorInputManager;
}

void EditorApplication::Initialize()
{
    mEditorWindow = new Window(1920, 1030, true);
    mRenderer = new Zephyrus::Render::RendererOpenGl();
    mSceneManager = new Zephyrus::Scenes::SceneManager(mRenderer);
    Zephyrus::Assets::AssetsManager::SetContext(mSceneManager);

    mImGuiEditorLayer = std::make_unique<ImGuiEditorLayer>();
    
    // For now
    //InputManager::Instance().SetContext(mSceneManager);

    if (mEditorWindow->Open(mTitle) && mRenderer->Initialize(*mEditorWindow) && Zephyrus::Render::TextRenderer::Instance().Init(*mEditorWindow))
    {
        glfwMaximizeWindow(mEditorWindow->GetGlfwWindow());
        mEditorInputManager = new InputManager(mEditorWindow->GetGlfwWindow());
        mEditorInputManager->SetPriority();
        
        // SDL_Surface* icon = IMG_Load("../Content/Sprites/Icons/ZephyrusLogo.png");
        // SDL_SetWindowIcon(mGameWindow->GetSdlWindow(), icon);
        // SDL_FreeSurface(icon);

        auto editorController = new Zephyrus::ActorComponent::EditorControllerActor(mSceneManager, *mSceneManager->GetActiveScene());
        mEditorController = editorController;
        mEditorController->GetComponentOfType<Zephyrus::ActorComponent::EditorControllerComponent>()->SetInputManager(mEditorInputManager);
        mEditorController->Start();

        mImGuiEditorLayer->InitializeImGui(mEditorWindow->GetGlfwWindow());
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
        GLFWwindow* win = mEditorWindow->GetGlfwWindow();
        glfwPollEvents();

        if (glfwWindowShouldClose(win)) mIsRunning = false;

        if (mEditorInputManager->HasPriority())
        {
            mEditorInputManager->UpdateKeysAndButtons();
        }
        else
        {
            mSceneManager->UpdateInput();
        }
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
    mEditorWindow->Close();
    EventSystem::ClearAllEvents();
}

