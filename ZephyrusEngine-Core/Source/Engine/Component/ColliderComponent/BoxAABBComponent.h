#pragma once
#include "ColliderComponent.h"
#include <utility>

class IRenderer;

/**
 * @brief Collider component representing an axis-aligned bounding box (AABB) for collision detection.
 * Used to check and resolve collisions for actors in the scene.
 */
class BoxAABBComponent : public ColliderComponent
{
public:
	BoxAABBComponent() = delete;
	BoxAABBComponent(Actor* pOwner);
	~BoxAABBComponent();

	void Deserialize(const rapidjson::Value& pData) override;

	static Component* Create(Actor* pOwner) { return new BoxAABBComponent(pOwner); }

	void OnStart() override;
	void Update() override;
	void OnEnd() override;

public:
	// Checks collision with another collider and outputs contact information
	bool CheckCollisionWith(ColliderComponent* other, ContactManifold& infosOut) override;

public:
	void SetShowInGame(bool pShowInGame);
	void SetSize(Vector3D pSize);

	// Draws the debug representation of the box collider
	void DebugDraw(IRenderer& pRenderer) override;

	Vector3D GetLastPosition();

	std::pair<Vector3D, Vector3D> GetCollisionPosition() const override { return mCollisionPosition; }

	// Returns the AABB struct of the collider
	AABB GetAABB() override;

	ColliderType GetColliderType() const override { return ColliderType::BoxAABB; }

private:
	// Checks collision specifically with another BoxAABBComponent
	bool CheckCollisionWithBoxAABB(BoxAABBComponent* pOther, ContactManifold& pInfosOut);

private:
	Vector3D mPosition;
	Vector3D mLastPosition;
	std::pair<Vector3D, Vector3D> mCollisionPosition;
	bool mShowInGame = true;
};
