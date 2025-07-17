#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Maths.h"

class MeshComponent : public Component
{
public:
	MeshComponent(Actor* pOwner, Mesh* pMesh, ShaderProgram* pProgram = nullptr);
	virtual ~MeshComponent();
	virtual void Draw(Matrix4DRow viewProj);
	virtual void SetMesh(Mesh& pMesh);
	void SetTextureIndex(unsigned int pTextureIndex);
	inline Mesh* GetMesh() const { return mMesh; }

	void SetShaderProgram(ShaderProgram pShaderProgram);

	inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }

	void SetTiling(Vector2D pTiling);

protected:
	Mesh* mMesh;
	Shader mVertexShader, mFragmentShader;
	ShaderProgram mShaderProgram;
	Vector2D mTiling;
	unsigned int mTextureIndex;
};

