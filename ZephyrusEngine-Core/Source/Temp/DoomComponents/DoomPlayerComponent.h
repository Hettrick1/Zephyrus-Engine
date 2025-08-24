#pragma once

#include "Component.h"
#include "FlipbookComponent.h"
#include "HudElements/HudText.h"
#include "HudElements/HudImage.h"
#include "Temp/DoomComponents/Weapons.h"

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
	Texture gunIcon;
	Texture shotgunIcon;
	std::vector<Texture*> mGunAnim;
	std::vector<Texture*> mShotgunAnim;
	int mGunAmo;
	int mHealth;
	int mArmor;
	Weapons mWeapon;
	float mDamageIndicatorAlpha;
public:
	DoomPlayerComponent(Actor* pOwner, int updateOder = 0);
	~DoomPlayerComponent();

	void Deserialize(const rapidjson::Value& pData) override;

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