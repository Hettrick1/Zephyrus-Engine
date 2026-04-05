#include "pch.h"
#include "DoomEnemyComponent.h"
#include "AssetsManager.h"
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

	void DoomEnemyComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
	}

	void DoomEnemyComponent::Serialize(Serialization::ISerializer& pWriter)
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
