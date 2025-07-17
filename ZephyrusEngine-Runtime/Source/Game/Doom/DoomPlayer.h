#pragma once

#include "FlipbookComponent.h"
#include "Actor.h"
#include "HudElements/HudText.h"
#include "HudElements/HudImage.h"
#include "Weapons.h"

class DoomPlayer : public Actor
{
public:
	DoomPlayer();
	~DoomPlayer();
	void Start() override;
	void Update() override;
	void Destroy() override;

	Actor* GetActorRef();
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
private:
	FlipbookComponent* mGun;
	HudText* mFpsText;
	HudText* mGunAmoText;
	HudText* mHealthText;
	HudText* mArmorText;
	HudImage* mWeaponIconImage;
	HudImage* mDamageIndicatorImage;
	Texture gunIcon;
	Texture shotgunIcon;
	std::vector<Texture*> mGunAnim;
	std::vector<Texture*> mShotgunAnim;
	int mGunAmo;
	int mHealth;
	int mArmor;
	Weapons mWeapon;
	float mDamageIndicatorAlpha;
};

