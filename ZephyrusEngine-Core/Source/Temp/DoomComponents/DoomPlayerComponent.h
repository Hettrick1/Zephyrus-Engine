#pragma once

#include "Component.h"
#include "FlipbookComponent.h"
#include "HudText.h"
#include "HudImage.h"
#include "Temp/DoomComponents/Weapons.h"

namespace Zephyrus::Assets
{
	class ITexture;
}

using Zephyrus::UI::HudText;
using Zephyrus::UI::HudImage;
using Zephyrus::UI::TextAlignment;

namespace Zephyrus::ActorComponent
{
	class DoomPlayerComponent : public Component
	{
	private:
		FlipbookComponent* mGun = nullptr;
		HudText* mFpsText = nullptr;
		HudText* mGunAmoText = nullptr;
		HudText* mHealthText = nullptr;
		HudText* mArmorText = nullptr;
		HudImage* mWeaponIconImage = nullptr;
		HudImage* mDamageIndicatorImage = nullptr;
		Assets::ITexture* gunIcon;
		Assets::ITexture* shotgunIcon;
		std::vector<Assets::ITexture*> mGunAnim;
		std::vector<Assets::ITexture*> mShotgunAnim;
		int mGunAmo;
		int mHealth;
		int mArmor;
		Weapons mWeapon;
		float mDamageIndicatorAlpha;
	public:
		DoomPlayerComponent(Actor* pOwner, int updateOder = 0);
		~DoomPlayerComponent();

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new DoomPlayerComponent(pOwner); }

		void OnStart() override;
		void Update() override;
		void OnEnd() override;

	public:
		void ChangeWeapon();
		void Shoot(int pAmoQuantity);
		void UseAmo(int pQuantity);
		void PickUpAmo(int pQuantity);
		void PickUpShield(int pQuantity);
		void PickUpHealth(int pQuantity);
		inline int GetAmo() const { return mGunAmo; }
		inline Weapons GetWeapon() const { return mWeapon; }
		void TakeDamages(int pQuantity);
	};
}