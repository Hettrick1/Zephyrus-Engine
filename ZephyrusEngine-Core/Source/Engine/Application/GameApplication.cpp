#include "pch.h"
#include "GameApplication.h"
#include "Log.h"
#include "RendererOpenGl.h"
#include "TextRenderer.h"
#include "SplashScreen.h"

#include <Bullet3Common/b3Scalar.h>
#include <Bullet3Common/b3Transform.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <iostream>


#include "Material/Material.h"
#include "ISerializationFactory.h"


namespace Zephyrus::Application {
    GameApplication::GameApplication(const std::string& pTitle, const std::string& pGameConfigFile)
        : mEditorConfigFile(pGameConfigFile), mProjectName(pTitle)
    {
        Zephyrus::Debug::Log::Init();

        Initialize();
    }

    GameApplication::~GameApplication()
    {
        delete mSceneManager;
        delete mRenderer;
        delete mGameWindow;
    }

    void GameApplication::Initialize()
    {
        mGameWindow = new Window(1920, 1080, true, false);
        mRenderer = new Zephyrus::Render::RendererOpenGl();

        mSceneManager = new Zephyrus::Scenes::SceneManager(mRenderer);
        Assets::AssetsManager::GetInstance().Initialize(mSceneManager);
        InitializeEditorConfig();

        if (mGameWindow->Open(mProjectName) && mRenderer->Initialize(*mGameWindow) && Zephyrus::Render::TextRenderer::Instance().Init(*mGameWindow)) {
#ifdef _DEBUG
            mSceneManager->LoadSceneFromFileId(mStartMapId, mRenderer);
#else
            mSceneManager->LoadSplashScreen(new Scenes::SplashScreen(mSceneManager, mStartMapId), mRenderer);
#endif
            Loop();
        }
    }

    void GameApplication::InitializeEditorConfig()
    {
        auto reader = mSceneManager->GetSerializationFactory()->CreateDeserializer();

        if (reader->LoadDocument(mEditorConfigFile))
        {
            if (auto projectName = reader->ReadString("gameName"))
            {
                mProjectName = *projectName;
            }
            if (auto startupMapId = reader->ReadString("startupMapId"))
            {
                mStartMapId = *startupMapId;
            }
        }
    }

    void GameApplication::Loop()
    {
        while (mIsRunning) {
            Timer::ComputeDeltaTime();
            Input();
            Update();
            Render();
            //Timer::DelayTime();
        }

        Close();
    }

    void GameApplication::Update()
    {
        mSceneManager->Update(Timer::deltaTime);
#ifdef _DEBUG
        mSceneManager->DebugUpdate();
#endif
    }

    void GameApplication::Render()
    {
        mSceneManager->RenderAll();
    }

    void GameApplication::Input()
    {
        GLFWwindow* window = mGameWindow->GetGlfwWindow();
        glfwPollEvents();

        if (glfwWindowShouldClose(window))
            mIsRunning = false;
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            mIsRunning = false;

        mSceneManager->UpdateInput();
    }

    void GameApplication::Close()
    {
        mSceneManager->Unload();
        mGameWindow->Close();
        Zephyrus::Debug::Log::Shutdown();
    }
}