#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "Physics/ICollisionListener.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderProgram.h"

class BasicSATCube : public Actor, public ICollisionListener
{
public:
	BasicSATCube(Vector3D pPos = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0), ShaderProgram* program = nullptr);
	~BasicSATCube();
	void Start() override;
	void Update() override;
	void Destroy() override;
public:
	void OnTriggerEnter(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerStay(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerExit(ColliderComponent* collider, HitResult* infos) override;
private:
	ShaderProgram* mShaderProgram;
};