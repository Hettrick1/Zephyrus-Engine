#pragma once
#include "Component.h"
#include "Maths.h"
#include "Interface/ICubeMapTexture.h"
#include "Interface/IShader.h"
#include "Interface/ITexture2D.h"
#include "Interface/IShaderProgram.h"
#include "RenderComponent.h"

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
		bool mIsSphere = false;
		std::vector<std::string> mTexturesPaths;

	public:
		SkySphereComponent(Actor* pOwner);
		virtual ~SkySphereComponent() override = default;

		void OnStart() override;
		void OnEnd() override;

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;
		static Component* Create(Actor* pOwner) { return new SkySphereComponent(pOwner); }

		void Draw(const Zephyrus::Render::IRenderer& pRenderer) override;

		std::vector<PropertyDescriptor> GetProperties() override;

		void SetTexturePaths(std::vector<std::string>& pTexturesPaths);

		inline Assets::IMesh* GetMesh() const { return mMesh; }
		inline bool GetIsSphere() const { return mIsSphere; }

		static int index;
	};
}