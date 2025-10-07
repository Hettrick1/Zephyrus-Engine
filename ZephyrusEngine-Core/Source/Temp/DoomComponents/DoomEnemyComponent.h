#pragma once

#include "Component.h"
#include "FlipbookComponent.h"

namespace Zephyrus::ActorComponent
{
	class DoomEnemyComponent : public Component
	{
	private:
		FlipbookComponent* mEnemyFb;
		FlipbookComponent* mSplashBlood;
		std::vector<Assets::ITexture*> EnemyShootAnim;
		std::vector<Assets::ITexture*> EnemyDieAnim;
		std::vector<Assets::ITexture*> EnemyExplodesAnim;
		std::vector<Assets::ITexture*> EnemyDamagesAnim;
		std::string mEnemyFbId;
		std::string mSplashBloodFbId;
		int mHealth;
		float mShootCouldown;
		bool mCanShoot, mIsDead;
	public:
		DoomEnemyComponent(Actor* pOwner, int updateOder = 0);
		~DoomEnemyComponent();

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new DoomEnemyComponent(pOwner); }

		void OnStart() override;
		void Update() override;
		void OnEnd() override;

		std::vector<PropertyDescriptor> GetProperties() override;

	public:
		void TakeDamage(int pDamages, int weapon);
	};
}

