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
public:
	SkySphereComponent(Actor* pOwner, bool isSphere = false, std::vector<std::string> textures = {}, ShaderProgram* pProgram = nullptr);
	virtual ~SkySphereComponent();

	// Draws the sky sphere or skybox using the given view-projection matrix
	virtual void Draw(Matrix4DRow viewProj);

	virtual void SetMesh(Mesh& pMesh);

	// Sets the index of the texture to use
	void SetTextureIndex(unsigned int pTextureIndex);

	inline Mesh* GetMesh() const { return mMesh; }

	// Sets the shader program used for rendering
	void SetShaderProgram(ShaderProgram pShaderProgram);

	inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }
	inline CubeTextureMap& GetCubeMap() { return mCubeMap; }
	inline unsigned int& GetTextureIndex() { return mTextureIndex; }
	inline VertexArray* GetVao() const { return mVao; }
	inline GLenum& GetTextureType() { return mTextureType; }

	// Sets the tiling factor for the sky texture
	void SetTiling(Vector2D pTiling);

	static int index;

protected:
	Mesh* mMesh;
	Shader mVertexShader, mFragmentShader, mTessellationControlShader, mTessellationEvalShader;
	ShaderProgram mShaderProgram;
	CubeTextureMap mCubeMap;
	Vector2D mTiling;
	unsigned int mTextureIndex;
	bool mIsSphere;
	VertexArray* mVao;
	GLenum mTextureType;
	std::vector<std::string> mTextureToLoad;
};