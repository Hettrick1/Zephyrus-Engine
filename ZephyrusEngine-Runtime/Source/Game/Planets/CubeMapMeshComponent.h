#pragma once
#include "Component.h"
#include "MeshComponent.h"
#include "CubeTextureMap.h"
#include "Mesh.h"
#include "Maths.h"

class CubeMapMeshComponent : public MeshComponent
{
public:
	CubeMapMeshComponent(Actor* pOwner, Mesh* pMesh, CubeTextureMap pCubeMap, ShaderProgram* pProgram = nullptr);
	virtual ~CubeMapMeshComponent();
	void Draw(Matrix4DRow viewProj) override;

	inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }

protected:
	CubeTextureMap mCubeMapTexture;
};