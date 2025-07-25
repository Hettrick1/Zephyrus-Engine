#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Maths.h"

class MeshComponent : public Component
{
protected:
	Mesh* mMesh = nullptr;
	Shader mVertexShader, mFragmentShader;
	ShaderProgram mShaderProgram;
	Vector2D mTiling;
	unsigned int mTextureIndex = 0;
public:
	MeshComponent(Actor* pOwner, Mesh* pMesh, ShaderProgram* pProgram = nullptr);
	virtual ~MeshComponent();
	virtual void Draw(const Matrix4DRow& pViewProj);
	virtual void SetMesh(Mesh& pMesh);
	void SetTextureIndex(unsigned int pTextureIndex);

	void SetShaderProgram(const ShaderProgram& pShaderProgram);

	void SetTiling(const Vector2D& pTiling);

	inline Mesh* GetMesh() const { return mMesh; }
	inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }
};

