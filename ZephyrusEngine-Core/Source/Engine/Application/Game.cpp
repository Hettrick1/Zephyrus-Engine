#include "Game.h"
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
#include "Utils/JSONUtils.h"


namespace Zephyrus::Application {
    Game::Game(const std::string& pTitle, const std::string& pStartupScene)
        : mIsRunning(true), mStartUpScene(pStartupScene), mInputManager(InputManager::Instance()), mTitle(pTitle)
    {
        Zephyrus::Debug::Log::Init();
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
        delete mSceneManager;
        delete mRenderer;
        delete mGameWindow;
    }

    void Game::Initialize()
    {
        mGameWindow = new Window(1920, 1080, false);
        mRenderer = new Zephyrus::Render::RendererOpenGl();

        mSceneManager = new Zephyrus::Scenes::SceneManager(mRenderer);
        Assets::AssetsManager::SetContext(mSceneManager);

        // For now
        InputManager::Instance().SetContext(mSceneManager);

        if (mGameWindow->Open(mTitle) && mRenderer->Initialize(*mGameWindow) && Zephyrus::Render::TextRenderer::Instance().Init(*mGameWindow)) {
#ifdef _DEBUG
            mSceneManager->LoadSceneWithFile(mStartUpScene, mRenderer);
#else
            mSceneManager->LoadSplashScreen(new Scenes::SplashScreen(mSceneManager, mStartUpScene), mRenderer);
#endif
            Loop();
        }
    }

    void Game::Loop()
    {
        //temp to create easily materials
        Zephyrus::Material::Material mat;
        mat.SetVertexShader(Assets::AssetsManager::LoadShader("../Content/Shaders/VertFrag/SkyBox.vert", ShaderType::VERTEX, "../Content/Shaders/VertFrag/SkyBox.vert"));
        mat.SetFragmentShader(Assets::AssetsManager::LoadShader("../Content/Shaders/VertFrag/SkyBox.frag", ShaderType::FRAGMENT, "../Content/Shaders/VertFrag/SkyBox.frag"));
        mat.SetTessControlShader(Assets::AssetsManager::LoadShader("../Content/Shaders/Tesselation/SkyBox.tesc", ShaderType::TESSELLATION_CONTROL, "../Content/Shaders/Tesselation/SkyBox.tesc"));
        mat.SetTessEvalShader(Assets::AssetsManager::LoadShader("../Content/Shaders/Tesselation/SkyBox.tese", ShaderType::TESSELLATION_EVALUATION, "../Content/Shaders/Tesselation/SkyBox.tese"));

        std::vector<std::string> paths =
        {
            "../Content/Sprites/SkyBox/DAY_CLOUDS_S.jpg",
            "../Content/Sprites/SkyBox/DAY_CLOUDS_N.jpg",
            "../Content/Sprites/SkyBox/DAY_CLOUDS_T.jpg",
            "../Content/Sprites/SkyBox/DAY_CLOUDS_B.jpg",
            "../Content/Sprites/SkyBox/DAY_CLOUDS_W.jpg",
            "../Content/Sprites/SkyBox/DAY_CLOUDS_E.jpg"
        };

        mat.SetTexture("albedo", Assets::AssetsManager::LoadCubemap(paths, paths[0]));

        Serialization::Json::JsonWriter writer;

        mat.Serialize(writer);

        writer.SaveDocument("../Content/Material/SkyBox.zpmat");

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
        mSceneManager->Update(Timer::deltaTime);
    }

    void Game::Render()
    {
        mSceneManager->RenderAll();
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
        mSceneManager->Unload();
        mGameWindow->Close();
        Zephyrus::Debug::Log::Shutdown();
    }
}