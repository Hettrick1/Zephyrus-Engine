#include "Game.h"
#include "Defs.h"
#include "Log.h"
#include "RendererSdl.h"
#include "RendererOpenGl.h"
#include "TextRenderer.h"
#include "SplashScreen.h"

Game::Game(const std::string& pTitle, const std::string& pStartupScene)
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

    std::string fullPath = "../Config/Game.config";

    std::ifstream file(fullPath);

    if (!file.is_open())
    {
        ZP_CORE_ERROR("Impossible to open the game.config : " + fullPath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonContent = buffer.str();

    rapidjson::Document doc;
    doc.Parse(jsonContent.c_str());

    if (doc.HasParseError()) {
        ZP_CORE_ERROR("Parsing JSON failed !");
        return;
    }

    if (doc.HasMember("gameName") && doc["gameName"].IsString())
    {
        mTitle = doc["gameName"].GetString();
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
    mGameWindow = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, false);
    mRenderer = new RendererOpenGl();
    if (mGameWindow->Open(mTitle) && mRenderer->Initialize(*mGameWindow) && TextRenderer::Instance().Init(*mGameWindow)) {
        #ifdef _DEBUG
            SceneManager::LoadSceneWithFile(mStartUpScene, mRenderer, false);
        #else
            SceneManager::LoadSplashScreen(new SplashScreen(mStartUpScene), mRenderer);
        #endif
        Loop();
    }
}

void Game::Loop()
{
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
    SceneManager::RenderAll();
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
    Zephyrus::Log::Shutdown();
}
