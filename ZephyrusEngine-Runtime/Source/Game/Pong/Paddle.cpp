#include "Paddle.h"

Paddle::Paddle(Vector2D paddlePos)
{
	mPaddlePos = paddlePos;
}

Paddle::~Paddle()
{
}

void Paddle::MovePlayer(float direction)
{
	if (direction > 0 && mPaddlePos.y > 0) {
		mPaddlePos.y -= mPaddleSpeed.y;
	}
	if (direction < 0 && mPaddlePos.y < 800 - mPaddleSize.y) {
		mPaddlePos.y += mPaddleSpeed.y;
	}
}

void Paddle::MoveIA(float speed)
{
	if (speed > 0) {
		if (mPaddlePos.y > 0) {
			mPaddlePos.y -= mPaddleSpeed.y;
		}
	}
	else if (speed < 0){
		if (mPaddlePos.y < 800 - mPaddleSize.y) {
			mPaddlePos.y += mPaddleSpeed.y;
		}
	}
}

Rectangle Paddle::GetPaddleRect()
{
	Rectangle rect{ mPaddlePos, mPaddleSize };
	return rect;
}
