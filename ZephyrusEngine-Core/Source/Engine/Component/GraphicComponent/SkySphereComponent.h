#pragma once
#include "Component.h"
#include "Texture.h"
#include "Maths.h"
#include "CubeTextureMap.h"
#include "Shader.h"
#include "ShaderProgram.h"

namespace Zephyrus::Render {
	class IMesh;
}

using Zephyrus::Assets::CubeTextureMap;
using Zephyrus::Render::ShaderProgram;
using Zephyrus::Render::Shader;
using Zephyrus::Render::ShaderType;
/**
 * @brief Component that manages and renders a sky sphere or skybox in the scene.
 * It handles the mesh, textures, shaders, and rendering logic for the sky.
 */
namespace Zephyrus::ActorComponent
{
	class SkySphereComponent : public Component
	{
	protected:
		Render::IMesh* mMesh = nullptr;
		Shader mVertexShader, mFragmentShader, mTescShader, mTeseShader;
		ShaderProgram mShaderProgram;
		CubeTextureMap mCubeMap;
		Vector2D mTiling;
		unsigned int mTextureIndex = 0;
		Zephyrus::Assets::Texture* mSphereTexture = nullptr;
		bool mIsSphere = false;
		GLenum mTextureType;

		std::vector<std::string> mTexturesPaths;

	public:
		SkySphereComponent(Actor* pOwner);
		virtual ~SkySphereComponent();

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;
		static Component* Create(Actor* pOwner) { return new SkySphereComponent(pOwner); }

		std::vector<PropertyDescriptor> GetProperties() override;

		// Sets the index of the texture to use
		void SetTextureIndex(unsigned int pTextureIndex);

		// Sets the shader program used for rendering
		void SetShaderProgram(const ShaderProgram& pShaderProgram);

		// Sets the tiling factor for the sky texture
		void SetTiling(const Vector2D& pTiling);

		void SetTexturePaths(std::vector<std::string>& pTexturesPaths);

		inline Render::IMesh* GetMesh() const { return mMesh; }
		inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }
		inline CubeTextureMap& GetCubeMap() { return mCubeMap; }
		inline unsigned int& GetTextureIndex() { return mTextureIndex; }
		inline GLenum& GetTextureType() { return mTextureType; }
		inline bool GetIsSphere() const { return mIsSphere; }
		inline Zephyrus::Assets::Texture* GetSphereTexture() const { return mSphereTexture; }

		static int index;
	};
}