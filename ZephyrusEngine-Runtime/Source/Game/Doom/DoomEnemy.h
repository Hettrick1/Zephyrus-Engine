#pragma once
#include "FlipbookComponent.h"
#include "Actor.h"


class DoomPlayer;

class DoomEnemy : public Actor
{
public:
	DoomEnemy(DoomPlayer* pPlayerRef, Vector3D pPos = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0));
	~DoomEnemy();
	void Start() override;
	void Update() override;
	void Destroy() override;

	void TakeDamage(int pDamages, int weapon);
private:
	FlipbookComponent* mEnemyFb;
	FlipbookComponent* mSplashBlood;
	std::vector<Texture*> EnemyShootAnim;
	std::vector<Texture*> EnemyDieAnim;
	std::vector<Texture*> EnemyExplodesAnim;
	std::vector<Texture*> EnemyDamagesAnim;
	DoomPlayer* mPlayerRef;
	int mHealth;
	float mShootCouldown;
	bool mCanShoot, mIsDead;
};