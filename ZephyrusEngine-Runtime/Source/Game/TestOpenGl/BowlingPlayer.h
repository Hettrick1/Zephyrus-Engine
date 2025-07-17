#pragma once
#include "Actor.h"
#include "BowlingBall.h"
#include "BallSpawnPoint.h"
#include "BowlingPin.h"
class BowlingPlayer : public Actor
{
public:
	BowlingPlayer();
	~BowlingPlayer();
	void Start() override;
	void Update() override;
	void Destroy() override;

	void IncrementIndicatorSize(float strengthPercent);
	void MoveSpawnPointX(float x);
	void ThrowBowlingBall(float strengthPercent);

	void RotateSpawnPointZ(float axis);
private:
	BowlingBall* mBall;
	BallSpawnPoint* mBallSpawnPoint;
	std::vector<BowlingPin*> mPins;
	Vector3D mSpawnPosition;
	bool mBeginTimer;
	float mTimer;
};

