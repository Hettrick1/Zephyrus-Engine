#include "DoomEnemyComponent.h"
#include "Assets.h"
#include "Scene.h"
#include "Timer.h"
#include "DebugRenderer.h"
#include "DebugLine.h"
#include "DoomPlayerComponent.h"
#include "SceneManager.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::ActorComponent
{
	const float damages = 20;
	const float weaponRange = 60;
	const float weaponSpreadAngle = 0.8;
	const float shootCooldown = 1;
	const float detectionRange = 800;

	DoomEnemyComponent::DoomEnemyComponent(Actor* pOwner, int updateOder)
		: Component(pOwner, "DoomEnemyComponent"), mEnemyFb(nullptr), mHealth(100), mShootCouldown(shootCooldown)
		, mCanShoot(false), mIsDead(false), mSplashBlood(nullptr)
	{
		EnemyShootAnim = {
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MShoot1.png", "Mshoot1"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MShoot2.png", "Mshoot2"),
		};
		EnemyDieAnim = {
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MShotDie1.png", "MShotDie1"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MShotDie2.png", "MShotDie2"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MShotDie3.png", "MShotDie3"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MShotDie4.png", "MShotDie4"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MShotDie5.png", "MShotDie5"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MShotDie6.png", "MShotDie6"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MShotDie7.png", "MShotDie7"),
		};
		EnemyExplodesAnim = {
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MExplodes1.png", "MExplodes1"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MExplodes2.png", "MExplodes2"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MExplodes3.png", "MExplodes3"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MExplodes4.png", "MExplodes4"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MExplodes5.png", "MExplodes5"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MExplodes6.png", "MExplodes6"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MExplodes7.png", "MExplodes7"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MExplodes8.png", "MExplodes8"),
			AssetsManager::LoadTexture("Sprites/Doom/Marine/MExplodes9.png", "MExplodes9"),
		};
		EnemyDamagesAnim = {
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_00.png", "MDamage0"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_0.png", "MDamage1"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_1.png", "MDamage2"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_2.png", "MDamage3"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_3.png", "MDamage4"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_4.png", "MDamage5"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_5.png", "MDamage6"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_6.png", "MDamage7"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_7.png", "MDamage8"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_8.png", "MDamage9"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_9.png", "MDamage10"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_10.png", "MDamage11"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_11.png", "MDamage12"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_12.png", "MDamage13"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_13.png", "MDamage14"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_14.png", "MDamage15"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_15.png", "MDamage16"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_16.png", "MDamage17"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_17.png", "MDamage18"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_18.png", "MDamage19"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_19.png", "MDamage20"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_20.png", "MDamage21"),
			AssetsManager::LoadTexture("Sprites/Doom/SplashBlood/1_21.png", "MDamage22"),
		};
	}

	DoomEnemyComponent::~DoomEnemyComponent()
	{
		mEnemyFb = nullptr;
		mSplashBlood = nullptr;
		EnemyShootAnim.clear();
		EnemyDieAnim.clear();
		EnemyExplodesAnim.clear();
		EnemyDamagesAnim.clear();
	}

	void DoomEnemyComponent::Deserialize(const rapidjson::Value& pData)
	{
		Component::Deserialize(pData);
	}

	void DoomEnemyComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
	{
		Component::Serialize(pWriter);
	}

	void DoomEnemyComponent::OnStart()
	{
		Component::OnStart();
		if (!mOwner->GetComponents().empty())
		{
			auto flipbooks = mOwner->GetAllComponentOfType<FlipbookComponent>();

			if (flipbooks.size() >= 2)
			{
				if (flipbooks[0]->HasTag("enemyFB"))
				{
					mEnemyFb = flipbooks[0];
					mSplashBlood = flipbooks[1];
					mEnemyFbId = mEnemyFb->GetId();
					mSplashBloodFbId = mSplashBlood->GetId();
				}
				else
				{
					mEnemyFb = flipbooks[1];
					mSplashBlood = flipbooks[0];
					mEnemyFbId = mEnemyFb->GetId();
					mSplashBloodFbId = mSplashBlood->GetId();
				}
			}
		}
	}

	void DoomEnemyComponent::Update()
	{
		Component::Update();

		if (mEnemyFb == nullptr)
		{
			if (!mEnemyFbId.empty())
			{
				mEnemyFb = dynamic_cast<FlipbookComponent*>(mOwner->GetComponentWithId(mEnemyFbId));
				if (!mEnemyFb)
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
		if (mSplashBlood == nullptr)
		{
			if (!mSplashBloodFbId.empty())
			{
				mSplashBlood = dynamic_cast<FlipbookComponent*>(mOwner->GetComponentWithId(mSplashBloodFbId));
				if (!mSplashBlood)
				{
					return;
				}
			}
			else
			{
				return;
			}
		}

		auto playerRef = mOwner->GetScene().GetPlayerRef();

		// bilboard
		//Vector3D camPos = SceneManager::ActiveScene->GetCameraManager()->GetActiveCamera()->GetWorldTransform().GetTranslation();
		Vector3D camPos = mOwner->GetSceneContext()->GetCameraManager()->GetActiveCamera()->GetWorldTransform().GetTranslation();
		Vector3D direction = camPos - mOwner->GetTransformComponent().GetPosition();

		float angleZ = zpMaths::ATan2(direction.y, direction.x);
		angleZ -= zpMaths::ToRad(90);
		Quaternion targetRotation = Quaternion(Vector3D::unitZ, angleZ);
		mOwner->SetRotation(targetRotation);

		if (!mIsDead)
		{
			float dist = (mOwner->GetTransformComponent().GetPosition() - playerRef->GetTransformComponent().GetPosition()).LengthSq();

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
				Vector3D start = mOwner->GetTransformComponent().GetPosition();
				start.z -= 0.0f;
				Vector3D baseDirection = mOwner->GetTransformComponent().GetWorldTransform().GetYAxis();

				const float spreadAngle = weaponSpreadAngle;
				const float range = weaponRange;
				float randomAngle = 0;
				randomAngle = zpMaths::RandomRange(-spreadAngle, spreadAngle);
				float randomRadians = zpMaths::ToRad(randomAngle);

				Matrix4DRow rotation = Matrix4DRow::CreateRotationZ(randomRadians);
				Vector3D dir = rotation.TransformVector(baseDirection);
				dir.Normalize();

				Vector3D end = start + dir * range;
				HitResult hit;
				mOwner->GetScene().GetPhysicWorld()->LineTrace(start, end, hit, mOwner);
				Zephyrus::Debug::DebugLine* line = new Zephyrus::Debug::DebugLine(start, end, hit);
				mOwner->GetScene().GetRenderer()->AddDebugLine(line);
				if (hit.HitActor != nullptr && hit.HitActor->HasTag("Player"))
				{
					playerRef->GetComponentOfType<DoomPlayerComponent>()->TakeDamages(damages);
				}
			}
		}
	}

	void DoomEnemyComponent::OnEnd()
	{
		Component::OnEnd();
	}

	std::vector<PropertyDescriptor> DoomEnemyComponent::GetProperties()
	{
		return {
			{"Enemy Fb : ", &mEnemyFbId, PropertyType::Component},
			{"Splashblood Fb : ", &mSplashBloodFbId, PropertyType::Component},
		};
	}

	void DoomEnemyComponent::TakeDamage(int pDamages, int weapon)
	{
		if (mEnemyFb == nullptr)
		{
			return;
		}
		if (mSplashBlood == nullptr)
		{
			return;
		}

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
			if (auto col = mOwner->GetComponentOfType<BulletColliderComponent>())
			{
				col->SetActive(false);
			}
		}
	}
}
