#include "DoomPlayer.h"
#include "DoomPC.h"
#include "CameraComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "Scene.h"
#include "MeshComponent.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "HudManager.h"
#include "Physics/CollisionManager.h"
#include "Physics/PhysicManager.h"
#include "BoxAABBComponent.h"
#include "DoomEnemy.h"
#include "SceneManager.h"
#include "Childs/LVLDoomMainMenu.h"

float bobingTime = 0;
const float gunDamages = 25;
const float shotgunDamages = 25;
const float gunRange = 30;
const float shotgunRange = 20;
const float shotgunSpreadAngle = 4;


DoomPlayer::DoomPlayer()
	: Actor(), mGun(nullptr), mGunAmo(50), mHealth(100), mArmor(100), 
	mFpsText(nullptr), mGunAmoText(nullptr), mHealthText(nullptr), mArmorText(nullptr), 
	mWeaponIconImage(nullptr), mWeapon(Weapons::Gun), mDamageIndicatorImage(nullptr), mDamageIndicatorAlpha(0.0f)
{
	Start();
	mScene.AddActor(this);
}

DoomPlayer::~DoomPlayer()
{
	delete mGun;
	delete mFpsText;
	delete mGunAmoText;
	delete mHealthText;
	delete mArmorText;
	delete mWeaponIconImage;
	delete mDamageIndicatorImage;
	mGunAnim.clear();
	mShotgunAnim.clear();
}

void DoomPlayer::Start()
{
	Actor::Start();
	RigidbodyComponent* rigidBody = new RigidbodyComponent(this);
	rigidBody->SetIsStatic(true);
	rigidBody->SetFriction(1.0);
	SetRigidBody(rigidBody);
	DoomPC* playerController = new DoomPC(this, 1);
	playerController->SetPlayerRef(this);
	Texture* doomHud = Assets::LoadTexture("Sprites/Doom/DoomHud.png", "doomHud");
	gunIcon = *Assets::LoadTexture("Sprites/Doom/DoomHudGunIcon.png", "gunIcon");
	shotgunIcon = *Assets::LoadTexture("Sprites/Doom/DoomHudShotGunIcon.png", "shotgunIcon");

	mGunAnim = {
		Assets::LoadTexture("Sprites/Doom/gun1.png", "gun1"),
		Assets::LoadTexture("Sprites/Doom/gun2.png", "gun2"),
		Assets::LoadTexture("Sprites/Doom/gun3.png", "gun3"),
		Assets::LoadTexture("Sprites/Doom/gun4.png", "gun4")
	};
	mShotgunAnim = {
		Assets::LoadTexture("Sprites/Doom/shotgun1.png", "shotgun1"),
		Assets::LoadTexture("Sprites/Doom/shotgun2.png", "shotgun2"),
		Assets::LoadTexture("Sprites/Doom/shotgun3.png", "shotgun3"),
		Assets::LoadTexture("Sprites/Doom/shotgun4.png", "shotgun4"),
		Assets::LoadTexture("Sprites/Doom/shotgun4.png", "shotgun5"),
		Assets::LoadTexture("Sprites/Doom/shotgun5.png", "shotgun6"),
		Assets::LoadTexture("Sprites/Doom/shotgun5.png", "shotgun7"),
		Assets::LoadTexture("Sprites/Doom/shotgun6.png", "shotgun8"),
		Assets::LoadTexture("Sprites/Doom/shotgun6.png", "shotgun9"),
		Assets::LoadTexture("Sprites/Doom/shotgun5.png", "shotgun10"),
		Assets::LoadTexture("Sprites/Doom/shotgun5.png", "shotgun11"),
		Assets::LoadTexture("Sprites/Doom/shotgun4.png", "shotgun12"),
		Assets::LoadTexture("Sprites/Doom/shotgun4.png", "shotgun13"),
	};

	CameraComponent* cameraComponent = new CameraComponent(this);
	cameraComponent->SetRelativePosition(Vector3D(0,0, 0.4));
	mGun = new FlipbookComponent(this, mGunAnim, 100000000);
	mGun->SetRelativePosition(Vector3D(0.0f, 2.0f, 0.2f));
	mGun->RelativeRotateX(90);
	mGun->SetAnimationFps(8);
	mGun->SetCullOff(true);
	 
	HudImage* doomHudImage = new HudImage(*doomHud, Vector2D(0, -920), Vector2D(10, 10));
	mWeaponIconImage = new HudImage(gunIcon, Vector2D(400, -980), Vector2D(10, 10));

	mFpsText = new HudText("AAAAAAAAA", Vector2D(-1900.0f, 1000.0f), 0.5f, Vector4D(1, 0, 1, 1));
	mGunAmoText = new HudText(std::to_string(mGunAmo), Vector2D(-825.0f, -930.0f), 1, Vector4D(0.7f, 0, 0, 1), TextAlignment::CENTER);
	mHealthText = new HudText(std::to_string(mHealth), Vector2D(-325.0f, -930.0f), 1, Vector4D(0.7f, 0, 0, 1), TextAlignment::CENTER);
	mArmorText = new HudText(std::to_string(mArmor), Vector2D(840.0f, -930.0f), 1, Vector4D(0.7f, 0, 0, 1), TextAlignment::CENTER);;

	Texture* damageIndicator = Assets::LoadTexture("Sprites/Doom/DamageIndicator.png", "DamageIndicator");
	mDamageIndicatorImage = new HudImage(*damageIndicator, Vector2D(0, 0), 2);
	mDamageIndicatorImage->SetTint(Vector4D(1.0, 1.0, 1.0, 0.0));

	BoxAABBComponent* bc = new BoxAABBComponent(this, 10, 0.1, Vector3D(0, 0, -0.12));
	bc->SetIsQuerry(true);

	SetTag("Player");
}

void DoomPlayer::Update()
{
	Actor::Update();
	Vector3D bobbing = mGun->GetRelativePosition();
	if (GetRigidBody()->GetVelocity().LengthSq() > 1) {

		bobingTime += Timer::deltaTime * 10.0f;

		bobbing.z -= Maths::Sin(bobingTime) * 0.005f;

		bobbing.x += Maths::Sin(bobingTime * 0.5f) * 0.005f;

		mGun->SetRelativePosition(bobbing);
	}
	else 
	{
		float lerpRelativeSpeed = 8;
		Vector3D lerpRelative = Vector3D::Lerp(mGun->GetRelativePosition(), Vector3D(0.0f, 2.0f, 0.2f), Timer::deltaTime * lerpRelativeSpeed);
		mGun->SetRelativePosition(lerpRelative);
		bobingTime = 0;
	}
	mFpsText->SetText("Fps : " + std::to_string(Timer::mFPS));

	if (mDamageIndicatorAlpha >= 0) 
	{
		mDamageIndicatorAlpha -= Timer::deltaTime;
		mDamageIndicatorImage->SetTint(Vector4D(1.0, 1.0, 1.0, mDamageIndicatorAlpha));
	}
}

void DoomPlayer::Destroy()
{
	Actor::Destroy();
}

Actor* DoomPlayer::GetActorRef()
{
	return static_cast<Actor*>(this);
}

void DoomPlayer::ChangeWeapon()
{
	if (mGun->IsAnimationEnded()) {
		if (mWeapon == Weapons::Gun)
		{
			mWeapon = Weapons::Shotgun;
		}
		else
		{
			mWeapon = Weapons::Gun;
		}
		switch (mWeapon)
		{
		case Weapons::Gun:
			mGun->SetAnimationFps(8);
			mGun->SetAnimationTextures(mGunAnim);
			mWeaponIconImage->SetTexture(gunIcon);
			break;
		case Weapons::Shotgun:
			mGun->SetAnimationFps(22);
			mGun->SetAnimationTextures(mShotgunAnim);
			mWeaponIconImage->SetTexture(shotgunIcon);
			break;
		}
	}
}

void DoomPlayer::Shoot(int pAmoQuantity)
{
	switch (mWeapon) {
		case Weapons::Gun:
		{
			const float range = gunRange;
			Vector3D start = GetTransformComponent().GetPosition();
			start.z -= 0.0f;
			Vector3D end = start + GetTransformComponent().GetWorldTransform().GetYAxis() * range;
			HitResult hit;
			PhysicManager::Instance().LineTrace(start, end, hit, this);
			DebugLine* line = new DebugLine(start, end, hit);
			GetScene().GetRenderer()->AddDebugLine(line);
			UseAmo(pAmoQuantity);

			if (hit.HitActor != nullptr && hit.HitActor->HasTag("Enemy"))
			{
				DoomEnemy* enemy = static_cast<DoomEnemy*>(hit.HitActor);
				enemy->TakeDamage(gunDamages, static_cast<int>(mWeapon));
			}

			break;
		}
		case Weapons::Shotgun:
		{
			Vector3D start = GetTransformComponent().GetPosition();
			start.z -= 0.0f;
			Vector3D baseDirection = GetTransformComponent().GetWorldTransform().GetYAxis();

			const float spreadAngle = shotgunSpreadAngle;
			const float range = shotgunRange;       
			float randomAngle = 0;

			for (int i = 0; i < 4; ++i)
			{
				randomAngle = Maths::RandomRange(-spreadAngle, spreadAngle);
				float randomRadians = Maths::ToRad(randomAngle);

				// Création d'une matrice de rotation autour de l'axe Z (vertical)
				Matrix4DRow rotation = Matrix4DRow::CreateRotationZ(randomRadians); 
				Vector3D dir = rotation.TransformVector(baseDirection);
				dir.Normalize();

				Vector3D end = start + dir * range;
				HitResult hit;
				PhysicManager::Instance().LineTrace(start, end, hit, this);
				DebugLine* line = new DebugLine(start, end, hit);
				GetScene().GetRenderer()->AddDebugLine(line);
				if (hit.HitActor != nullptr && hit.HitActor->HasTag("Enemy"))
				{
					DoomEnemy* enemy = static_cast<DoomEnemy*>(hit.HitActor);
					enemy->TakeDamage(shotgunDamages, static_cast<int>(mWeapon));
				}
			}
			UseAmo(pAmoQuantity);
			break;
		}
	}
}

void DoomPlayer::UseAmo(int pQuantity)
{
	mGunAmo -= pQuantity;
	if (mGunAmo < 0)
	{
		mGunAmo = 0;
	}
	mGunAmoText->SetText(std::to_string(mGunAmo));
}

void DoomPlayer::PickUpAmo(int pQuantity)
{
	mGunAmo += pQuantity;
	mGunAmoText->SetText(std::to_string(mGunAmo));
}

void DoomPlayer::PickUpShield(int pQuantity)
{
	mArmor += pQuantity;
	mArmorText->SetText(std::to_string(mArmor));
}

void DoomPlayer::PickUpHealth(int pQuantity)
{
	mHealth += pQuantity;
	mHealthText->SetText(std::to_string(mHealth));
}

void DoomPlayer::TakeDamages(int pQuantity)
{
	mDamageIndicatorAlpha = 1.0;
	if (mArmor > 0)
	{
		mArmor -= pQuantity;
		if (mArmor < 0)
		{
			mArmor = 0;
		}
	}
	else 
	{
		mHealth -= pQuantity;
		if (mHealth <= 0)
		{
			SceneManager::LoadScene(new LVLDoomMainMenu());
			return;
		}
	}
	mArmorText->SetText(std::to_string(mArmor));
	mHealthText->SetText(std::to_string(mHealth));
}
