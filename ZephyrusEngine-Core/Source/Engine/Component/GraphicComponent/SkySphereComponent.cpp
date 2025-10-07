#include "SkySphereComponent.h"
#include "Scene.h"
#include "IRenderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Vertex.h"
#include "SceneManager.h"
#include "Interface/IMesh.h"

#include "ComponentFactory.h"
#include "JSONUtils.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::ActorComponent
{
	int SkySphereComponent::index = 0;

	SkySphereComponent::SkySphereComponent(Actor* pOwner)
		: Component(pOwner, "SkySphereComponent"), mMesh(nullptr), mTiling(1), mIsSphere(false), mTextureType(GL_TEXTURE_2D)
	{
		mOwner->GetScene().GetRenderer()->AddSkySphere(this);
	}

	SkySphereComponent::~SkySphereComponent()
	{
		mOwner->GetScene().GetRenderer()->AddSkySphere(nullptr);
	}

	std::vector<PropertyDescriptor> SkySphereComponent::GetProperties()
	{
		std::vector<PropertyDescriptor> prop;
		if (mIsSphere)
		{
			prop = {
				{"Texture : ", &mSphereTexture, PropertyType::Texture, true}
			};
		}
		else
		{
			prop = {
				{"Textures : ", &mCubeMap, PropertyType::CubeMap}
			};
		}

		return prop;
	}

	void SkySphereComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
		if (auto isSphere = pReader.ReadBool("isSphere"))
		{
			if (*isSphere) 
			{
				mIsSphere = true;
				mVertexShader = *AssetsManager::LoadShader("VertFrag/SkySphere.vert", ShaderType::VERTEX, "SkySphereVert");
				mFragmentShader = *AssetsManager::LoadShader("VertFrag/SkySphere.frag", ShaderType::FRAGMENT, "SkySphereFrag");
				mShaderProgram = *AssetsManager::LoadShaderProgram({ &mVertexShader, &mFragmentShader }, "skySphereSP");

				Zephyrus::Assets::Texture* tex = nullptr;
				if (auto sphereTexture = pReader.ReadArrayString("textures"))
				{
					const auto& arr = *sphereTexture;
					if (!arr.empty())
					{
						tex = AssetsManager::LoadTexture(arr[0], arr[0]);
						mTexturesPaths.push_back(arr[0]);
						mSphereTexture = tex;
						mTextureIndex = tex->GetId();
						mMesh = AssetsManager::LoadMesh("sphere.obj", "sphere");
						mTextureType = GL_TEXTURE_2D;
					}
					else
					{
						ZP_CORE_ERROR("Textures array must contain at least one string!");
					}
				}
			}
			else
			{
				mIsSphere = false;
				mVertexShader = *AssetsManager::LoadShader("VertFrag/SkyBox.vert", ShaderType::VERTEX, "SkyBoxVert");
				mFragmentShader = *AssetsManager::LoadShader("VertFrag/SkyBox.frag", ShaderType::FRAGMENT, "SkyBoxFrag");
				mTescShader = *AssetsManager::LoadShader("Tesselation/SkyBox.tesc", ShaderType::TESSELLATION_CONTROL, "SkyBoxTesc");
				mTeseShader = *AssetsManager::LoadShader("Tesselation/SkyBox.tese", ShaderType::TESSELLATION_EVALUATION, "SkyBoxTese");

				mShaderProgram = *AssetsManager::LoadShaderProgram({ &mVertexShader, &mTescShader, &mTeseShader, &mFragmentShader }, "skyboxSP");

				mMesh = AssetsManager::LoadMesh("cube.obj", "cube");

				std::vector<std::string> faces;
				mTexturesPaths.clear();

				if (auto cubeTexture = pReader.ReadArrayString("textures"))
				{
					bool cubemapSuccess = false;
					const auto& arr = *cubeTexture;

					if (!arr.empty() && arr.size() == 6)
					{
						for (auto& element : arr)
						{
							faces.push_back(element);
							mTexturesPaths.push_back(element);
						}
						cubemapSuccess = mCubeMap.CreateCubeTextureMap(faces);
						ZP_CORE_ASSERT(cubemapSuccess, "Cubemap creation failed!");
						mTextureIndex = mCubeMap.GetID();
						mTextureType = GL_TEXTURE_CUBE_MAP;
					}
					else
					{
						ZP_CORE_ASSERT(arr.size() == 6, "Textures array must contain 6 strings!");
					}
				}
			}
		}
	}

	void SkySphereComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);

		pWriter.WriteBool("isSphere", mIsSphere);

		pWriter.BeginArray("textures");
		for (auto texture : mTexturesPaths)
		{
			pWriter.PushString(texture);
		}
		pWriter.EndArray();

		Component::EndSerialize(pWriter);
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

	void SkySphereComponent::SetTexturePaths(std::vector<std::string>& pTexturesPaths)
	{
		mTexturesPaths = pTexturesPaths;
	}
}