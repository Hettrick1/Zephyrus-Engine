#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderProgram.h"
#include "CubeMapMeshComponent.h"

class Planet : public Actor
{
public:
	Planet(Vector3D pPos = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0), ShaderProgram* program = nullptr, CubeTextureMap* pCubemap = nullptr);
	~Planet();
	void Start() override;
	void Update() override;
	void Destroy() override;

private:
	ShaderProgram* mShaderProgram;
	CubeTextureMap* mCubemap;
};