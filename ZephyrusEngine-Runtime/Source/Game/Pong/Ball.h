#pragma once

#include "Vector2D.h"
#include "Rectangle.h"
#include "Log.h"
#include "Paddle.h"

class Ball
{
private:
	Vector2D mBallPos = { 400, 250 };
	Vector2D mBallSpeed = { -4, 4};
	float mBallRadius = 30;
	bool mIsDead = false;
public:
	Ball();
	~Ball();
	void Move();
	void CheckCollision(const int WIDTH, const int HEIGHT, Paddle* paddleL, Paddle* paddleR);
	void BounceX();
	void BounceY();
	Vector2D GetPosition();
	Vector2D GetBallSpeed();
	Rectangle GetBallRect() const;
	float GetSize();
	bool GetIsDead();
};

