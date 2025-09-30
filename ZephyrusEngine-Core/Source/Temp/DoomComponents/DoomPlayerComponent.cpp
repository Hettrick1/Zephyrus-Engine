#include "DoomPlayerComponent.h"
#include "Log.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "DoomEnemyComponent.h"

float bobingTime = 0;
const float gunDamages = 25;
const float shotgunDamages = 25;
const float gunRange = 30;
const float shotgunRange = 20;
const float shotgunSpreadAngle = 4;

DoomPlayerComponent::DoomPlayerComponent(Actor* pOwner, int updateOder)
	: Component(pOwner, "DoomPlayerComponent", updateOder), mGunAmo(50), mHealth(100), mArmor(100), mWeapon(Weapons::Gun), mDamageIndicatorAlpha(0.0f)
{
}

DoomPlayerComponent::~DoomPlayerComponent()
{
	mGunAnim.clear();
	mShotgunAnim.clear();
}

void DoomPlayerComponent::Deserialize(const rapidjson::Value& pData)
{
	Component::Deserialize(pData);
}

void DoomPlayerComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
	Component::Serialize(pWriter);
}

void DoomPlayerComponent::OnStart()
{
	Component::OnStart();

	mOwner->GetScene().GetRenderer()->GetDebugRenderer()->SetDrawDebug(false);

	mGun = mOwner->GetComponentOfType<FlipbookComponent>();
	ZP_ASSERT(mGun, "FlipbookComponent not found !");

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
	mGun->SetAnimationTextures(mGunAnim);
	mGun->SetAnimationFps(8);
	mGun->SetCullOff(true);
	mGun->SetDrawOrder(100000000);

	mWeaponIconImage = new HudImage(gunIcon, Vector2D(400, -980), Vector2D(10, 10));
	mWeaponIconImage->SetDrawOrder(100.0f);
	HudImage* doomHudImage = new HudImage(*doomHud, Vector2D(0, -920), Vector2D(10, 10));
	doomHudImage->SetDrawOrder(10.0f);
	mFpsText = new HudText("AAAAAAAAA", Vector2D(-1900.0f, 1000.0f), 0.5f, Vector4D(1, 0, 1, 1));
	mFpsText->SetDrawOrder(101.0f);
	mGunAmoText = new HudText(std::to_string(mGunAmo), Vector2D(-825.0f, -930.0f), 1, Vector4D(0.7f, 0, 0, 1), TextAlignment::CENTER);
	mGunAmoText->SetDrawOrder(101.0f);
	mHealthText = new HudText(std::to_string(mHealth), Vector2D(-325.0f, -930.0f), 1, Vector4D(0.7f, 0, 0, 1), TextAlignment::CENTER);
	mHealthText->SetDrawOrder(101.0f);
	mArmorText = new HudText(std::to_string(mArmor), Vector2D(840.0f, -930.0f), 1, Vector4D(0.7f, 0, 0, 1), TextAlignment::CENTER);
	mArmorText->SetDrawOrder(101.0f);

	Texture* damageIndicator = Assets::LoadTexture("Sprites/Doom/DamageIndicator.png", "DamageIndicator");
	mDamageIndicatorImage = new HudImage(*damageIndicator, Vector2D(0, 0), 2);
	mDamageIndicatorImage->SetDrawOrder(0.0f);
	mDamageIndicatorImage->SetTint(Vector4D(1.0, 1.0, 1.0, 0.0));
	CameraComponent* cam = mOwner->GetComponentOfType<CameraComponent>();
	if (cam)
	{
		mGun->SetParent(cam);
	}
}

void DoomPlayerComponent::Update()
{
	Component::Update();
	Vector3D bobbing = mGun->GetRelativePosition();
	if (mOwner->GetRigidBody()->GetVelocity().LengthSq() > 1) {

		bobingTime += Timer::deltaTime * 10.0f;

		bobbing.z -= Maths::Sin(bobingTime) * 0.005f;

		bobbing.x += Maths::Sin(bobingTime * 0.5f) * 0.005f;

		mGun->SetRelativePosition(bobbing);
	}
	else
	{
		float lerpRelativeSpeed = 15;
		Vector3D lerpRelative = Vector3D::Lerp(mGun->GetRelativePosition(), Vector3D(0.0f, 2.0f, -0.2f), Timer::deltaTime * lerpRelativeSpeed);
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

void DoomPlayerComponent::OnEnd()
{
}

void DoomPlayerComponent::ChangeWeapon()
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

void DoomPlayerComponent::Shoot(int pAmoQuantity)
{
	CameraComponent* cam = mOwner->GetComponentOfType<CameraComponent>();
	switch (mWeapon) {
	case Weapons::Gun:
	{
		const float range = gunRange;
		Vector3D start = mOwner->GetTransformComponent().GetPosition();
		start.z -= 0.0f;
		Vector3D end = start + cam->GetWorldTransform().GetYAxis() * range;
		HitResult hit;
		SceneManager::ActiveScene->GetPhysicWorld()->LineTrace(start, end, hit, mOwner);
		DebugLine* line = new DebugLine(start, end, hit);
		mOwner->GetScene().GetRenderer()->AddDebugLine(line);
		UseAmo(pAmoQuantity);

		if (hit.HitActor != nullptr && hit.HitActor->HasTag("Enemy"))
		{
			auto enemyComp = hit.HitActor->GetComponentOfType<DoomEnemyComponent>();
			if (enemyComp != nullptr)
			{
				enemyComp->TakeDamage(gunDamages, static_cast<int>(mWeapon));
			}
		}

		break;
	}
	case Weapons::Shotgun:
	{
		Vector3D start = mOwner->GetTransformComponent().GetPosition();
		start.z -= 0.0f;
		Vector3D baseDirection = cam->GetWorldTransform().GetYAxis();

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
			SceneManager::ActiveScene->GetPhysicWorld()->LineTrace(start, end, hit, mOwner);
			DebugLine* line = new DebugLine(start, end, hit);
			mOwner->GetScene().GetRenderer()->AddDebugLine(line);
			if (hit.HitActor != nullptr && hit.HitActor->HasTag("Enemy"))
			{
				auto enemyComp = hit.HitActor->GetComponentOfType<DoomEnemyComponent>();
				if (enemyComp != nullptr)
				{
					enemyComp->TakeDamage(gunDamages, static_cast<int>(mWeapon));
				}
			}
		}
		UseAmo(pAmoQuantity);
		break;
	}
	}
}

void DoomPlayerComponent::UseAmo(int pQuantity)
{
	mGunAmo -= pQuantity;
	if (mGunAmo < 0)
	{
		mGunAmo = 0;
	}
	mGunAmoText->SetText(std::to_string(mGunAmo));
}

void DoomPlayerComponent::PickUpAmo(int pQuantity)
{
	mGunAmo += pQuantity;
	mGunAmoText->SetText(std::to_string(mGunAmo));
}

void DoomPlayerComponent::PickUpShield(int pQuantity)
{
	mArmor += pQuantity;
	mArmorText->SetText(std::to_string(mArmor));
}

void DoomPlayerComponent::PickUpHealth(int pQuantity)
{
	mHealth += pQuantity;
	mHealthText->SetText(std::to_string(mHealth));
}

void DoomPlayerComponent::TakeDamages(int pQuantity)
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
			SceneManager::LoadSceneWithFile("../Content/Maps/DoomMainMenu.zpmap", nullptr, true);
			return;
		}
	}
	mArmorText->SetText(std::to_string(mArmor));
	mHealthText->SetText(std::to_string(mHealth));
}
