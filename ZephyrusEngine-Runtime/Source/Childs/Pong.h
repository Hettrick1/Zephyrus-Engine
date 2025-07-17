#pragma once
#include "Scene.h"
#include "Game/Pong/Paddle.h"
#include "Game/Pong/Ball.h"

class Pong : public Scene
{
public:
	Pong();
	void Start(IRenderer* renderer) override;
	void Update() override;
	void Render() override;
	void Close() override;

public :
	void ResetGame();
private:
	Paddle* mPaddleRight;
	Paddle* mPaddleLeft;
	Ball* mBall;
	int mDirection; // -1 = down || 1 = up || 0 = stop
};

