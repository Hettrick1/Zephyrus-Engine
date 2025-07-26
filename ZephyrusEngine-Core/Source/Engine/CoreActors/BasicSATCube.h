#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "Physics/ICollisionListener.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderProgram.h"

class BasicSATCube : public Actor, public ICollisionListener
{
private:
	ShaderProgram* mShaderProgram;
public:
	BasicSATCube(const Vector3D& pPos = 0, const Vector3D& pSize = 1, const Quaternion& pRotation = Quaternion(0, 0), ShaderProgram* pProgram = nullptr);
	~BasicSATCube();
	void Start() override;
	void Update() override;
	void Destroy() override;
};