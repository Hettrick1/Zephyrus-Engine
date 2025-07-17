#include "Ball.h"

Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::Move()
{
	mBallPos.x += mBallSpeed.x;
	mBallPos.y += mBallSpeed.y;
}

void Ball::CheckCollision(const int WIDTH, const int HEIGHT, Paddle* paddleL, Paddle* paddleR)
{
    if (mBallPos.x < 0 && mBallSpeed.x < 0 || mBallPos.x + mBallRadius > WIDTH && mBallSpeed.x > 0) 
    {
        mIsDead = true;
    }
    if (mBallPos.y < 0 && mBallSpeed.y < 0 || mBallPos.y + mBallRadius > HEIGHT && mBallSpeed.y > 0) 
    {
        BounceY();
    }
    if (mBallPos.x <= (paddleL->GetPaddlePos().x + paddleL->GetPaddleSize().x) && 
        mBallPos.y >= paddleL->GetPaddlePos().y && 
        mBallPos.y + mBallRadius < paddleL->GetPaddlePos().y + paddleL->GetPaddleSize().y && 
        mBallSpeed.x < 0)
    {
        BounceX();
    }
    if (mBallPos.x + mBallRadius >= paddleR->GetPaddlePos().x && mBallSpeed.x > 0 && mBallPos.y >= paddleR->GetPaddlePos().y && mBallPos.y + mBallRadius < paddleR->GetPaddlePos().y + paddleR->GetPaddleSize().y && mBallSpeed.x > 0)
    {
        BounceX();
    }
}

void Ball::BounceX()
{
    mBallSpeed.x *= -1.05f;
}

void Ball::BounceY()
{
    mBallSpeed.y *= -1.0f;
}

Vector2D Ball::GetPosition()
{
	return mBallPos;
}

Vector2D Ball::GetBallSpeed()
{
    return mBallSpeed;
}

Rectangle Ball::GetBallRect() const
{
    Rectangle rect{ mBallPos, {mBallRadius, mBallRadius} };
	return rect;
}

float Ball::GetSize()
{
    return mBallRadius;
}

bool Ball::GetIsDead()
{
    return mIsDead;
}
