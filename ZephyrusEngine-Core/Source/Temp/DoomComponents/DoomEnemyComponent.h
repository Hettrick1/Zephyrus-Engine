#pragma once

#include "Component.h"
#include "FlipbookComponent.h"

class DoomEnemyComponent : public Component
{
private:
	FlipbookComponent* mEnemyFb;
	FlipbookComponent* mSplashBlood;
	std::vector<Texture*> EnemyShootAnim;
	std::vector<Texture*> EnemyDieAnim;
	std::vector<Texture*> EnemyExplodesAnim;
	std::vector<Texture*> EnemyDamagesAnim;
	std::string mEnemyFbId;
	std::string mSplashBloodFbId;
	int mHealth;
	float mShootCouldown;
	bool mCanShoot, mIsDead;
public:
	DoomEnemyComponent(Actor* pOwner, int updateOder = 0);
	~DoomEnemyComponent();

	void Deserialize(const rapidjson::Value& pData) override;
	void Serialize(Serialization::Json::JsonWriter& pWriter) override;

	static Component* Create(Actor* pOwner) { return new DoomEnemyComponent(pOwner); }

	void OnStart() override;
	void Update() override;
	void OnEnd() override;

	std::vector<PropertyDescriptor> GetProperties() override;

public:
	void TakeDamage(int pDamages, int weapon);
};

