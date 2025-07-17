#include "Game.h"
#include "Defs.h"
#include "Log.h"
#include "RendererSdl.h"
#include "RendererOpenGl.h"
#include "TextRenderer.h"
#include "SplashScreen.h"

Game::Game(std::string title, Scene* pStartupScene)
    : mIsRunning(true), mStartUpScene(pStartupScene), mInputManager(InputManager::Instance()), mPhysicManager(PhysicManager::Instance())
    , mCameraManager(CameraManager::Instance())
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
    }
    else
    {
        Log::Info("SDL initialization succeeded!");
    }

    Initialize();
}

Game::~Game()
{
    delete mRenderer;
    delete mGameWindow;
}

void Game::Initialize()
{
    mGameWindow = new Window(WINDOW_WIDTH, WINDOW_HEIGHT);
    mRenderer = new RendererOpenGl();
    if (mGameWindow->Open() && mRenderer->Initialize(*mGameWindow) && TextRenderer::Instance().Init(*mGameWindow)) {
        #ifdef _DEBUG
            SceneManager::LoadScene(mStartUpScene);
        #else
            SceneManager::LoadScene(new SplashScreen(mStartUpScene));
        #endif
        Loop();
    }
}

void Game::Loop()
{
    SceneManager::StartScene(mRenderer);

    while (mIsRunning) {
        Timer::ComputeDeltaTime();
        Input();
        Update(); 
        Render();
        Timer::DelayTime();
    }

    Close();
}

void Game::Update()
{
    mPhysicManager.Update();
    mCameraManager.UpdateCurrentCamera();
    SceneManager::Update();
}

void Game::Render()
{
    SceneManager::Render();
}

void Game::Input()
{
    if (mIsRunning) {  
        while (SDL_PollEvent(&mSdlEvent)) { 
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

void Game::Close()
{
    SceneManager::Unload();
    mGameWindow->Close();
}
