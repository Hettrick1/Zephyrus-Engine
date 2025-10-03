#pragma once
#include "Component.h"
#include "Texture.h"
#include "Mesh.h"
#include "Maths.h"
#include "CubeTextureMap.h"

using Zephyrus::Assets::CubeTextureMap;
using Zephyrus::Assets::Mesh;
using Zephyrus::Assets::VertexArray;

/**
 * @brief Component that manages and renders a sky sphere or skybox in the scene.
 * It handles the mesh, textures, shaders, and rendering logic for the sky.
 */
namespace Zephyrus::ActorComponent
{
	class SkySphereComponent : public Component
	{
	protected:
		Mesh* mMesh = nullptr;
		Shader mVertexShader, mFragmentShader, mTescShader, mTeseShader;
		ShaderProgram mShaderProgram;
		CubeTextureMap mCubeMap;
		Vector2D mTiling;
		unsigned int mTextureIndex = 0;
		Zephyrus::Assets::Texture* mSphereTexture = nullptr;
		bool mIsSphere = false;
		VertexArray* mVao = nullptr;
		GLenum mTextureType;

		std::vector<std::string> mTexturesPaths;

	public:
		SkySphereComponent(Actor* pOwner);
		virtual ~SkySphereComponent();

		void Deserialize(const rapidjson::Value& pData) override;
		void Serialize(Serialization::Json::JsonWriter& pWriter) override;
		static Component* Create(Actor* pOwner) { return new SkySphereComponent(pOwner); }

		void OnEnd() override;

		std::vector<PropertyDescriptor> GetProperties() override;

		// Sets the index of the texture to use
		void SetTextureIndex(unsigned int pTextureIndex);

		// Sets the shader program used for rendering
		void SetShaderProgram(const ShaderProgram& pShaderProgram);

		// Sets the tiling factor for the sky texture
		void SetTiling(const Vector2D& pTiling);

		void SetTexturePaths(std::vector<std::string>& pTexturesPaths);

		inline Mesh* GetMesh() const { return mMesh; }
		inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }
		inline CubeTextureMap& GetCubeMap() { return mCubeMap; }
		inline unsigned int& GetTextureIndex() { return mTextureIndex; }
		inline VertexArray* GetVao() const { return mVao; }
		inline GLenum& GetTextureType() { return mTextureType; }
		inline bool GetIsSphere() const { return mIsSphere; }
		inline Zephyrus::Assets::Texture* GetSphereTexture() const { return mSphereTexture; }

		static int index;
	};
}