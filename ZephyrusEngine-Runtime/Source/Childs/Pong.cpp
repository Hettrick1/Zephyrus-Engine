#include "Pong.h"


Pong::Pong() : Scene()
{
    mDirection = 0;
    mPaddleLeft = new Paddle({ 30, 400 });
    mPaddleRight = new Paddle({ 730, 400 });
    mBall = new Ball();
}

void Pong::Start(IRenderer* renderer)
{
	Scene::Start(renderer);
}

void Pong::Update()
{
    mBall->CheckCollision(800, 800, mPaddleLeft, mPaddleRight);
    mBall->Move();
    float speed = (mPaddleRight->GetPaddlePos().y + mPaddleRight->GetPaddleSize().y / 2 - mBall->GetSize() / 2)- mBall->GetPosition().y;
    mPaddleRight->MoveIA(speed);
    mPaddleLeft->MovePlayer(static_cast<float>(mDirection));
    if (mBall->GetIsDead()) {
        ResetGame();
    }
}

void Pong::Render()
{
    mRenderer->BeginDraw();
    Rectangle rectToDraw = mBall->GetBallRect();
    //mRenderer->DrawRect(rectToDraw);
    rectToDraw = mPaddleRight->GetPaddleRect();
    //mRenderer->DrawRect(rectToDraw);
    rectToDraw = mPaddleLeft->GetPaddleRect();
    //mRenderer->DrawRect(rectToDraw);    
    mRenderer->Draw();
    mRenderer->EndDraw();
}

//void Pong::OnInput(SDL_Event event)
//{
//    if (event.type == SDL_KEYDOWN) {
//        switch (event.key.keysym.sym)
//        {
//        case SDLK_w:
//            mDirection = 1;
//            break;
//        case SDLK_s:
//            mDirection = -1;
//            break;
//        default:
//            mDirection = 0;
//            break;
//        } 
//    }
//    else if (event.type == SDL_KEYUP){
//        switch (event.key.keysym.sym)
//        {
//        case SDLK_w:
//            mDirection = 0;
//            break;
//        case SDLK_s:
//            mDirection = 0;
//            break;
//        }
//    }
//}

void Pong::Close()
{
}

void Pong::ResetGame()
{
    mDirection = 0;
    mPaddleLeft = new Paddle({ 30, 400 });
    mPaddleRight = new Paddle({ 730, 400 });
    mBall = new Ball();
}
