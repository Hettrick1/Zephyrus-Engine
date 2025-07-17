#include "SkySphereComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "SceneManager.h"

int SkySphereComponent::index = 0;

SkySphereComponent::SkySphereComponent(Actor* pOwner, bool isSphere, std::vector<std::string> textures, ShaderProgram* pProgram)
	: Component(pOwner), mMesh(nullptr), mTextureIndex(0), mTiling(1), mIsSphere(isSphere), mTextureType(GL_TEXTURE_2D), mVao(nullptr), mTextureToLoad(textures)
{
	mOwner->GetScene().GetRenderer()->AddSkySphere(this);
	if (pProgram == nullptr)
	{
		if (isSphere) 
		{
			mVertexShader.Load("VertFrag/SkySphere.vert", ShaderType::VERTEX);
			mFragmentShader.Load("VertFrag/SkySphere.frag", ShaderType::FRAGMENT);
			mShaderProgram.Compose({ &mVertexShader, &mFragmentShader });
		}
		else
		{
			mVertexShader.Load("VertFrag/SkyBox.vert", ShaderType::VERTEX);
			mFragmentShader.Load("VertFrag/SkyBox.frag", ShaderType::FRAGMENT);
			mTessellationControlShader.Load("Tesselation/SkyBox.tesc", ShaderType::TESSELLATION_CONTROL);
			mTessellationEvalShader.Load("Tesselation/SkyBox.tese", ShaderType::TESSELLATION_EVALUATION);

			mShaderProgram.Compose({ &mVertexShader, &mTessellationControlShader, &mTessellationEvalShader, &mFragmentShader });
		}
	}
	else {
		mShaderProgram = *pProgram;
	}
	if (isSphere)
	{
		Texture* tex = nullptr;
		if (mTextureToLoad.empty()) {
			tex = Assets::LoadTexture(*SceneManager::ActiveScene->GetRenderer(), "../Imports/Sprites/LakeSkyTexture.png", "LakeSkyTexture");
		}
		else {
			tex = Assets::LoadTexture(*SceneManager::ActiveScene->GetRenderer(), mTextureToLoad[0], "skysphere" + index);
			index++;
		}
		mTextureIndex = tex->GetId();
		mMesh = Assets::LoadMesh("../Imports/Meshes/sphere.obj", "sphere");
		mVao = mMesh->GetVao();
		mTextureType = GL_TEXTURE_2D;
	}
	else 
	{
		mMesh = Assets::LoadMesh("../Imports/Meshes/cube.obj", "cube");
		mVao = mMesh->GetVao();
		std::vector<std::string> faces;
		if (mTextureToLoad.empty() || mTextureToLoad.size() < 6) {
			faces =
			{
				"../Imports/Sprites/SkyBox/DAY_CLOUDS_S.jpg",
				"../Imports/Sprites/SkyBox/DAY_CLOUDS_N.jpg",
				"../Imports/Sprites/SkyBox/DAY_CLOUDS_T.jpg",
				"../Imports/Sprites/SkyBox/DAY_CLOUDS_B.jpg",
				"../Imports/Sprites/SkyBox/DAY_CLOUDS_W.jpg",
				"../Imports/Sprites/SkyBox/DAY_CLOUDS_E.jpg",
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

void SkySphereComponent::Draw(Matrix4DRow viewProj)
{
}

void SkySphereComponent::SetMesh(Mesh& pMesh)
{
}

void SkySphereComponent::SetTextureIndex(unsigned int pTextureIndex)
{
}

void SkySphereComponent::SetShaderProgram(ShaderProgram pShaderProgram)
{
}

void SkySphereComponent::SetTiling(Vector2D pTiling)
{
}
