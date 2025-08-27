#include "SkySphereComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "SceneManager.h"

#include "ComponentFactory.h"
#include "JSONUtils.h"

int SkySphereComponent::index = 0;

SkySphereComponent::SkySphereComponent(Actor* pOwner)
	: Component(pOwner), mMesh(nullptr), mTiling(1), mIsSphere(false), mTextureType(GL_TEXTURE_2D), mVao(nullptr)
{
	mOwner->GetScene().GetRenderer()->AddSkySphere(this);
}

SkySphereComponent::~SkySphereComponent()
{
}

void SkySphereComponent::Deserialize(const rapidjson::Value& pData)
{
	Component::Deserialize(pData);
	if (pData.HasMember("isSphere") && pData["isSphere"].IsBool() && pData["isSphere"].GetBool())
	{
		mVertexShader = *Assets::LoadShader("VertFrag/SkySphere.vert", ShaderType::VERTEX, "SkySphereVert");
		mFragmentShader = *Assets::LoadShader("VertFrag/SkySphere.frag", ShaderType::FRAGMENT, "SkySphereFrag");
		mShaderProgram = *Assets::LoadShaderProgram({ &mVertexShader, &mFragmentShader }, "skySphereSP");

		Texture* tex = nullptr;
		if (pData.HasMember("textures") && pData["textures"].IsArray())
		{
			const auto& arr = pData["textures"].GetArray();
			if (!arr.Empty() && arr[0].IsString())
			{
				tex = Assets::LoadTexture(arr[0].GetString(), "skysphere");
			}
			else
			{
				ZP_CORE_ERROR("Textures array must contain at least one string!");
			}
		}
		mTextureIndex = tex->GetId();
		mMesh = Assets::LoadMesh("sphere.obj", "sphere");
		mVao = mMesh->GetVao();
		mTextureType = GL_TEXTURE_2D;
	}
	else if (pData.HasMember("isSphere") && pData["isSphere"].IsBool() && !pData["isSphere"].GetBool())
	{
		mVertexShader = *Assets::LoadShader("VertFrag/SkyBox.vert", ShaderType::VERTEX, "SkyBoxVert");
		mFragmentShader = *Assets::LoadShader("VertFrag/SkyBox.frag", ShaderType::FRAGMENT, "SkyBoxFrag");
		mTescShader = *Assets::LoadShader("Tesselation/SkyBox.tesc", ShaderType::TESSELLATION_CONTROL, "SkyBoxTesc");
		mTeseShader = *Assets::LoadShader("Tesselation/SkyBox.tese", ShaderType::TESSELLATION_EVALUATION, "SkyBoxTese");

		mShaderProgram = *Assets::LoadShaderProgram({ &mVertexShader, &mTescShader, &mTeseShader, &mFragmentShader }, "skyboxSP");

		mMesh = Assets::LoadMesh("cube.obj", "cube");
		mVao = mMesh->GetVao();
		std::vector<std::string> faces;

		if (pData.HasMember("textures") && pData["textures"].IsArray())
		{
			bool cubemapSuccess = false;
			const auto& arr = pData["textures"].GetArray();
			
			if (!arr.Empty() && arr.Size() == 6)
			{
				for (auto& element : arr) 
				{
					if (element.IsString())
					{
						faces.push_back(element.GetString());
					}
				}
				cubemapSuccess = mCubeMap.CreateCubeTextureMap(faces);
				ZP_CORE_ASSERT(cubemapSuccess, "Cubemap creation failed!");
				mTextureIndex = mCubeMap.GetID();
				mTextureType = GL_TEXTURE_CUBE_MAP;
			}
			else
			{
				ZP_CORE_ASSERT(arr.Size() == 6, "Textures array must contain 6 strings!");
			}
		}
	}
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
