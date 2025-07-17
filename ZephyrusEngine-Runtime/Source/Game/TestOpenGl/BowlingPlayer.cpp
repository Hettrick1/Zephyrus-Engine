#include "BowlingPlayer.h"
#include "BowlingPC.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "Timer.h"

BowlingPlayer::BowlingPlayer()
	: Actor(), mBall(nullptr), mBallSpawnPoint(nullptr), mBeginTimer(false), mTimer(0)
{
}

BowlingPlayer::~BowlingPlayer()
{
	if (mBall != nullptr)
	{
		mBall->Destroy();
	}
	if (mBallSpawnPoint != nullptr)
	{
		mBallSpawnPoint->Destroy();
	}
	mPins.clear();
}

void BowlingPlayer::Start()
{
	Actor::Start();
	CameraComponent* cc = new CameraComponent(this);
	BowlingPC* pc = new BowlingPC(this, 100);
	pc->SetMovementSpeed(5);

	mBall = new BowlingBall(0.0f, 1.3f);
	mBall->Start();
	mBall->SetPosition(Vector3D(-0.3f, -30.0f, -3.6f));
	mBall->GetComponentOfType<RigidbodyComponent>()->SetMass(5);
	mBall->GetComponentOfType<RigidbodyComponent>()->SetBounciness(0.8f);
	mBallSpawnPoint = new BallSpawnPoint(Vector3D(0.0f,-29.5f, -1.0f), Vector3D(0.1f, 0.5f, 0.1f));
	mBallSpawnPoint->Start();
	GetScene().AddActor(mBall);
	GetScene().AddActor(mBallSpawnPoint);

	mSpawnPosition = mBall->GetTransformComponent().GetPosition();

	BowlingPin* quille3 = new BowlingPin();
	GetScene().AddActor(quille3);
	quille3->GetTransformComponent().SetSize(0.8f);
	quille3->SetPosition(Vector3D(3.0f, 36.0f, -4.0f));
	quille3->Start();
	quille3->GetComponentOfType<RigidbodyComponent>()->SetMass(0.5);

	BowlingPin* quille4 = new BowlingPin();
	GetScene().AddActor(quille4);
	quille4->GetTransformComponent().SetSize(0.8f);
	quille4->SetPosition(Vector3D(-3.0f, 36.0f, -4.0f));
	quille4->Start();
	quille4->GetComponentOfType<RigidbodyComponent>()->SetMass(0.5);

	BowlingPin* quille5 = new BowlingPin();
	GetScene().AddActor(quille5);
	quille5->GetTransformComponent().SetSize(0.8f);
	quille5->SetPosition(Vector3D(0.0f, 36.0f, -4.0f));
	quille5->Start();
	quille5->GetComponentOfType<RigidbodyComponent>()->SetMass(0.5);

	BowlingPin* quille = new BowlingPin();
	GetScene().AddActor(quille);
	quille->GetTransformComponent().SetSize(0.8f);
	quille->SetPosition(Vector3D(1.5f, 33.0f, -4.0f));
	quille->Start();
	quille->GetComponentOfType<RigidbodyComponent>()->SetMass(0.5);

	BowlingPin* quille1 = new BowlingPin();
	GetScene().AddActor(quille1);
	quille1->GetTransformComponent().SetSize(0.8f);
	quille1->SetPosition(Vector3D(-1.5f, 33.0f, -4.0f));
	quille1->Start();
	quille1->GetComponentOfType<RigidbodyComponent>()->SetMass(0.5);

	BowlingPin* quille2 = new BowlingPin();
	GetScene().AddActor(quille2);
	quille2->GetTransformComponent().SetSize(0.8f);
	quille2->SetPosition(Vector3D(0.0f, 30.0f, -4.0f));
	quille2->Start();
	quille2->GetComponentOfType<RigidbodyComponent>()->SetMass(0.5);

	mPins.push_back(quille);
	mPins.push_back(quille1);
	mPins.push_back(quille2);
	mPins.push_back(quille3);
	mPins.push_back(quille4);
	mPins.push_back(quille5);
}

void BowlingPlayer::Update()
{
	Actor::Update();
	if (mBeginTimer)
	{
		mTimer += Timer::deltaTime;
	}

	if (mTimer >= 5.0)
	{
		mBeginTimer = false;
		mTimer = 0;
		mBall->SetPosition(mSpawnPosition);
		for (auto& pin : mPins)
		{
			pin->SetPosition(pin->GetInitialPos());
			pin->GetRigidBody()->SetVelocity(Vector3D::zero);
		}
	}
}

void BowlingPlayer::Destroy()
{
	Actor::Destroy();
}

void BowlingPlayer::IncrementIndicatorSize(float strengthPercent)
{
	float y = 2.5f * strengthPercent + 0.5f;
	mBallSpawnPoint->GetTransformComponent().SetSize(Vector3D(0.1f, y, 0.1f));
}

void BowlingPlayer::MoveSpawnPointX(float x)
{
	Vector3D spawnPoint = Vector3D(x, mBallSpawnPoint->GetTransformComponent().GetPosition().y, mBallSpawnPoint->GetTransformComponent().GetPosition().z);
	mSpawnPosition.x = x;
	mBallSpawnPoint->SetPosition(spawnPoint);
}

void BowlingPlayer::ThrowBowlingBall(float strengthPercent)
{
	if (!mBeginTimer)
	{
		Vector3D impulseDirection(1, 0, 0);

		Quaternion rotation = mBallSpawnPoint->GetTransformComponent().GetRotation();

		Vector3D rotatedDirection = rotation * impulseDirection;

		float strength = 200.0f * strengthPercent;
		Vector3D impulse = rotatedDirection * strength;

		mBall->SetPosition(mSpawnPosition);
		mBall->GetComponentOfType<RigidbodyComponent>()->AddImpulse(impulse);
		mBeginTimer = true;
	}
}

void BowlingPlayer::RotateSpawnPointZ(float axis)
{
	mBallSpawnPoint->GetTransformComponent().RotateZ(axis);
}
