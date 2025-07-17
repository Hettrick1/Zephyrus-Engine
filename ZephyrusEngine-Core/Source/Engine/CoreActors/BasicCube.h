#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "Physics/ICollisionListener.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderProgram.h"

/**
 * @brief Represents a basic cube actor in the scene, with collision and shader support.
 * Inherits from Actor and ICollisionListener to provide basic 3D cube functionality,
 * including collision event handling and rendering with a shader program.
 */
class BasicCube : public Actor, public ICollisionListener
{
public:
	BasicCube(Vector3D pPos = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0), ShaderProgram* program = nullptr);
	~BasicCube();

	void Start() override;
	void Update() override;
	void Destroy() override;

public:
	// Called when another collider enters this cube's trigger
	void OnTriggerEnter(ColliderComponent* collider, HitResult* infos) override;

	// Called when another collider stays within this cube's trigger
	void OnTriggerStay(ColliderComponent* collider, HitResult* infos) override;

	// Called when another collider exits this cube's trigger
	void OnTriggerExit(ColliderComponent* collider, HitResult* infos) override;

private:
	ShaderProgram* mShaderProgram;
};