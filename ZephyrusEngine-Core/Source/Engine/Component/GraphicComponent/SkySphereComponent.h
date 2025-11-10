#pragma once
#include "Component.h"
#include "Maths.h"
#include "Interface/ICubeMapTexture.h"
#include "Interface/IShader.h"
#include "Interface/ITexture2D.h"
#include "Interface/IShaderProgram.h"
#include "RenderComponent.h"

// TODO : Remove this include
#include "glew.h"

namespace Zephyrus::Assets {
	class IMesh;
}

using Zephyrus::Render::ShaderType;
/**
 * @brief Component that manages and renders a sky sphere or skybox in the scene.
 * It handles the mesh, textures, shaders, and rendering logic for the sky.
 */
namespace Zephyrus::ActorComponent
{
	class SkySphereComponent : public RenderComponent
	{
	protected:
		Assets::IMesh* mMesh{ nullptr };
		Render::IShader* mVertexShader{ nullptr };
		Render::IShader* mFragmentShader{ nullptr };
		Render::IShader* mTescShader{ nullptr };
		Render::IShader* mTeseShader{ nullptr };
		Render::IShaderProgram* mShaderProgram{ nullptr };
		Assets::ICubeMapTexture* mCubeMap{ nullptr };
		Vector2D mTiling;
		unsigned int mTextureIndex = 0;
		Zephyrus::Assets::ITexture2D* mSphereTexture{ nullptr };
		bool mIsSphere = false;
		GLenum mTextureType;

		std::vector<std::string> mTexturesPaths;

	public:
		SkySphereComponent(Actor* pOwner);
		virtual ~SkySphereComponent();

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;
		static Component* Create(Actor* pOwner) { return new SkySphereComponent(pOwner); }

		void Draw(const Zephyrus::Render::IRenderer& pRenderer) override;

		std::vector<PropertyDescriptor> GetProperties() override;

		// Sets the index of the texture to use
		void SetTextureIndex(unsigned int pTextureIndex);

		// Sets the shader program used for rendering
		void SetShaderProgram(Render::IShaderProgram* pShaderProgram);

		// Sets the tiling factor for the sky texture
		void SetTiling(const Vector2D& pTiling);

		void SetTexturePaths(std::vector<std::string>& pTexturesPaths);

		inline Assets::IMesh* GetMesh() const { return mMesh; }
		inline Render::IShaderProgram* GetShaderProgram() { return mShaderProgram; }
		inline Assets::ICubeMapTexture* GetCubeMap() { return mCubeMap; }
		inline unsigned int& GetTextureIndex() { return mTextureIndex; }
		inline GLenum& GetTextureType() { return mTextureType; }
		inline bool GetIsSphere() const { return mIsSphere; }
		inline Zephyrus::Assets::ITexture2D* GetSphereTexture() const { return mSphereTexture; }

		static int index;
	};
}