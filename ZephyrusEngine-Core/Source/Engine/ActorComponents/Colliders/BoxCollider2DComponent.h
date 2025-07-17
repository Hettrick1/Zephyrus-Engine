#pragma once

#include "ColliderComponent.h"
#include "Vector3D.h"

/**
 * @brief 2D box collider component for collision detection in the engine.
 * This component allows an Actor to interact with other colliders using a rectangular (box) shape.
 * It supports collision checks, debug rendering, and size management.
 * 
 * TO DO : It may need to be patch, I'm not sure it's still working.
 */
class BoxCollider2DComponent : public ColliderComponent
{
public:
	BoxCollider2DComponent() = delete;
	BoxCollider2DComponent(Actor* pOwner, int pUpdateOder, Vector3D pSize = 1);
	~BoxCollider2DComponent();

	void OnStart() override;
	void Update() override;
	void OnEnd() override;

public:
	// Checks collision with another collider and fills contact information
	bool CheckCollisionWith(ColliderComponent* other, ContactManifold& infosOut) override;

public:
	// Checks collision specifically with another BoxCollider2DComponent
	bool CheckCollisionWithBox(BoxCollider2DComponent* other);

public:
	// Sets whether the collider should be shown in game (for debug)
	void SetShowInGame(bool pShowInGame);
	void SetSize(Vector3D pSize);
private:
	Vector3D mPosition;
	Vector3D mSize;
	bool mShowInGame;
};
