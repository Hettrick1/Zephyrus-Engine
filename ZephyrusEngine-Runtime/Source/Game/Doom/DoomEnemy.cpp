#include "DoomEnemy.h"
#include "Assets.h"
#include "Scene.h"
#include "BoxAABBComponent.h"
#include "CameraManager.h"
#include "CameraComponent.h"
#include "Timer.h"
#include "Physics/CollisionManager.h"
#include "Physics/PhysicManager.h"
#include "DebugRenderer.h"
#include "DebugLine.h"
#include "DoomPlayer.h"

const float damages = 20;
const float weaponRange = 60;
const float weaponSpreadAngle = 0.8;
const float shootCooldown = 1;
const float detectionRange = 800;

DoomEnemy::DoomEnemy(DoomPlayer* pPlayerRef, Vector3D pPos, Vector3D pSize, Quaternion pRotation)
	: Actor(pPos, pSize, pRotation), mPlayerRef(pPlayerRef), mEnemyFb(nullptr), mHealth(100), mShootCouldown(shootCooldown)
	, mCanShoot(false), mIsDead(false), mSplashBlood(nullptr)
{
	Start();
	mScene.AddActor(this);
}

DoomEnemy::~DoomEnemy()
{
	mPlayerRef = nullptr;
	EnemyShootAnim.clear();
	EnemyDieAnim.clear();
	EnemyExplodesAnim.clear();
	EnemyDamagesAnim.clear();
	delete mEnemyFb;
	delete mSplashBlood;
}

void DoomEnemy::Start()
{
	Actor::Start();
	EnemyShootAnim = {
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MShoot1.png", "Mshoot1"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MShoot2.png", "Mshoot2"),
	};
	EnemyDieAnim = {
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MShotDie1.png", "MShotDie1"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MShotDie2.png", "MShotDie2"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MShotDie3.png", "MShotDie3"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MShotDie4.png", "MShotDie4"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MShotDie5.png", "MShotDie5"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MShotDie6.png", "MShotDie6"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MShotDie7.png", "MShotDie7"),
	};
	EnemyExplodesAnim = {
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MExplodes1.png", "MExplodes1"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MExplodes2.png", "MExplodes2"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MExplodes3.png", "MExplodes3"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MExplodes4.png", "MExplodes4"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MExplodes5.png", "MExplodes5"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MExplodes6.png", "MExplodes6"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MExplodes7.png", "MExplodes7"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MExplodes8.png", "MExplodes8"),
		Assets::LoadTexture("../Imports/Sprites/Doom/Marine/MExplodes9.png", "MExplodes9"),
	};
	EnemyDamagesAnim = {
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_00.png", "MDamage0"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_0.png", "MDamage1"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_1.png", "MDamage2"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_2.png", "MDamage3"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_3.png", "MDamage4"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_4.png", "MDamage5"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_5.png", "MDamage6"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_6.png", "MDamage7"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_7.png", "MDamage8"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_8.png", "MDamage9"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_9.png", "MDamage10"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_10.png", "MDamage11"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_11.png", "MDamage12"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_12.png", "MDamage13"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_13.png", "MDamage14"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_14.png", "MDamage15"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_15.png", "MDamage16"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_16.png", "MDamage17"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_17.png", "MDamage18"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_18.png", "MDamage19"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_19.png", "MDamage20"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_20.png", "MDamage21"),
		Assets::LoadTexture("../Imports/Sprites/Doom/SplashBlood/1_21.png", "MDamage22"),
	};
	mEnemyFb = new FlipbookComponent(this, EnemyShootAnim, 100);
	mEnemyFb->RelativeRotateX(90);
	mEnemyFb->SetAnimationFps(4);
	mEnemyFb->SetRelativeSize(2);

	mSplashBlood = new FlipbookComponent(this, EnemyDamagesAnim, 1);
	mSplashBlood->SetRelativePosition(Vector3D(0, 0.1, 0.3));
	mSplashBlood->RelativeRotateX(90);
	mSplashBlood->SetAnimationFps(40);
	mSplashBlood->SetRelativeSize(1.3);

	SetTag("Enemy");

	BoxAABBComponent* bc = new BoxAABBComponent(this, 10, Vector3D(0.4,0.4,1.0), Vector3D(0, 0, 0));
}

void DoomEnemy::Update()
{
	Actor::Update();

	// bilboard
	Vector3D camPos = CameraManager::Instance().GetCurrentCamera()->GetWorldTransform().GetTranslation();
	Vector3D direction = camPos - mTransformComponent.GetPosition();

	float angleZ = Maths::ATan2(direction.y, direction.x);
	angleZ -= Maths::ToRad(90);
	Quaternion targetRotation = Quaternion(Vector3D::unitZ, angleZ);
	mTransformComponent.SetRotation(targetRotation);

	if (!mIsDead)
	{
		float dist = (GetTransformComponent().GetPosition() - mPlayerRef->GetTransformComponent().GetPosition()).LengthSq();

		if (dist < detectionRange) {
			mCanShoot = true;
		}
		else {
			mCanShoot = false;
		}
	}

	if (mCanShoot)
	{
		if (mShootCouldown < 0.4 && mShootCouldown > 0.3)
		{
			mEnemyFb->PlayAnimation();
		}

		if (mShootCouldown > 0.0) {
			mShootCouldown -= Timer::deltaTime;
		}
		else {
			mShootCouldown = shootCooldown;
			Vector3D start = GetTransformComponent().GetPosition();
			start.z -= 0.0f;
			Vector3D baseDirection = GetTransformComponent().GetWorldTransform().GetYAxis();

			const float spreadAngle = weaponSpreadAngle;
			const float range = weaponRange;
			float randomAngle = 0;
			randomAngle = Maths::RandomRange(-spreadAngle, spreadAngle);
			float randomRadians = Maths::ToRad(randomAngle);

			Matrix4DRow rotation = Matrix4DRow::CreateRotationZ(randomRadians);
			Vector3D dir = rotation.TransformVector(baseDirection);
			dir.Normalize();

			Vector3D end = start + dir * range;
			HitResult hit;
			PhysicManager::Instance().LineTrace(start, end, hit, this);
			DebugLine* line = new DebugLine(start, end, hit);
			GetScene().GetRenderer()->AddDebugLine(line);
			if (hit.HitActor != nullptr && hit.HitActor->GetTag() == "Player")
			{
				mPlayerRef->TakeDamages(damages);
			}
		}
	}
}

void DoomEnemy::Destroy()
{
	Actor::Destroy();
}

void DoomEnemy::TakeDamage(int pDamages, int weapon)
{
	mSplashBlood->PlayAnimation();
	mHealth -= pDamages;
	if (mHealth <= 0)
	{
		switch (weapon)
		{
		case 0:
			mEnemyFb->SetAnimationTextures(EnemyDieAnim);
			mEnemyFb->SetAnimationFps(10);
			break;
		case 1:
			mEnemyFb->SetAnimationTextures(EnemyExplodesAnim);
			mEnemyFb->SetAnimationFps(15);
			break;
		}
		mCanShoot = false;
		mIsDead = true;
		mEnemyFb->PlayAnimation();
		mEnemyFb->SetCanPlay(false);
		GetComponentOfType<BoxAABBComponent>()->SetActive(false);
	}
}
