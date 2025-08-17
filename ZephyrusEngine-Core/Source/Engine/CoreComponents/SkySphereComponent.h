#pragma once
#include "Component.h"
#include "../Graphics/Texture.h"
#include "Mesh.h"
#include "Maths.h"
#include "CubeTextureMap.h"

/**
 * @brief Component that manages and renders a sky sphere or skybox in the scene.
 * It handles the mesh, textures, shaders, and rendering logic for the sky.
 */
class SkySphereComponent : public Component
{
protected:
	Mesh* mMesh = nullptr;
	Shader mVertexShader, mFragmentShader, mTescShader, mTeseShader;
	ShaderProgram mShaderProgram;
	CubeTextureMap mCubeMap;
	Vector2D mTiling;
	unsigned int mTextureIndex = 0;
	bool mIsSphere = false;
	VertexArray* mVao = nullptr;
	GLenum mTextureType;
public:
	SkySphereComponent(Actor* pOwner);
	virtual ~SkySphereComponent();

	void Deserialize(const rapidjson::Value& pData) override;

	static Component* Create(Actor* pOwner) { return new SkySphereComponent(pOwner); }
	static bool registered;

	// Sets the index of the texture to use
	void SetTextureIndex(unsigned int pTextureIndex);

	// Sets the shader program used for rendering
	void SetShaderProgram(const ShaderProgram& pShaderProgram);

	// Sets the tiling factor for the sky texture
	void SetTiling(const Vector2D& pTiling);

	inline Mesh* GetMesh() const { return mMesh; }
	inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }
	inline CubeTextureMap& GetCubeMap() { return mCubeMap; }
	inline unsigned int& GetTextureIndex() { return mTextureIndex; }
	inline VertexArray* GetVao() const { return mVao; }
	inline GLenum& GetTextureType() { return mTextureType; }

	static int index;
};