#include "SkySphereComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "SceneManager.h"

int SkySphereComponent::index = 0;

SkySphereComponent::SkySphereComponent(Actor* pOwner, bool pIsSphere, const std::vector<std::string>& pTextures, ShaderProgram* pProgram)
	: Component(pOwner), mMesh(nullptr), mTiling(1), mIsSphere(pIsSphere), mTextureType(GL_TEXTURE_2D), mVao(nullptr), mTextureToLoad(pTextures)
{
	mOwner->GetScene().GetRenderer()->AddSkySphere(this);
	if (pProgram == nullptr)
	{
		if (pIsSphere) 
		{
			mVertexShader = *Assets::LoadShader("VertFrag/SkySphere.vert", ShaderType::VERTEX, "SkySphereVert");
			mFragmentShader = *Assets::LoadShader("VertFrag/SkySphere.frag", ShaderType::FRAGMENT, "SkySphereFrag");
			mShaderProgram.Compose({ &mVertexShader, &mFragmentShader });
		}
		else
		{
			mVertexShader = *Assets::LoadShader("VertFrag/SkyBox.vert", ShaderType::VERTEX, "SkyBoxVert");
			mFragmentShader = *Assets::LoadShader("VertFrag/SkyBox.frag", ShaderType::FRAGMENT, "SkyBoxFrag");
			mTescShader = *Assets::LoadShader("Tesselation/SkyBox.tesc", ShaderType::TESSELLATION_CONTROL, "SkyBoxTesc");
			mTeseShader = *Assets::LoadShader("Tesselation/SkyBox.tese", ShaderType::TESSELLATION_EVALUATION, "SkyBoxTese");

			mShaderProgram.Compose({ &mVertexShader, &mTescShader, &mTeseShader, &mFragmentShader });
		}
	}
	else {
		mShaderProgram = *pProgram;
	}
	if (pIsSphere)
	{
		Texture* tex = nullptr;
		if (mTextureToLoad.empty()) {
			tex = Assets::LoadTexture("Sprites/LakeSkyTexture.png", "LakeSkyTexture");
		}
		else {
			tex = Assets::LoadTexture(mTextureToLoad[0], "skysphere" + index);
			index++;
		}
		mTextureIndex = tex->GetId();
		mMesh = Assets::LoadMesh("sphere.obj", "sphere");
		mVao = mMesh->GetVao();
		mTextureType = GL_TEXTURE_2D;
	}
	else 
	{
		mMesh = Assets::LoadMesh("cube.obj", "cube");
		mVao = mMesh->GetVao();
		std::vector<std::string> faces;
		if (mTextureToLoad.empty() || mTextureToLoad.size() < 6) {
			faces =
			{
				"Sprites/SkyBox/DAY_CLOUDS_S.jpg",
				"Sprites/SkyBox/DAY_CLOUDS_N.jpg",
				"Sprites/SkyBox/DAY_CLOUDS_T.jpg",
				"Sprites/SkyBox/DAY_CLOUDS_B.jpg",
				"Sprites/SkyBox/DAY_CLOUDS_W.jpg",
				"Sprites/SkyBox/DAY_CLOUDS_E.jpg",
			};
		}
		else
		{
			faces = mTextureToLoad;
		}
        
        mCubeMap.CreateCubeTextureMap(faces);
        mTextureIndex = mCubeMap.GetID();
        mTextureType = GL_TEXTURE_CUBE_MAP;
	}
}

SkySphereComponent::~SkySphereComponent()
{
}

void SkySphereComponent::SetTextureIndex(unsigned int pTextureIndex)
{
	mTextureIndex = pTextureIndex;
}

void SkySphereComponent::SetShaderProgram(const ShaderProgram& pShaderProgram)
{
	mShaderProgram = pShaderProgram;
}

void SkySphereComponent::SetTiling(const Vector2D& pTiling)
{
	mTiling = pTiling;
}
