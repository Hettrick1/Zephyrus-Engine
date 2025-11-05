#pragma once
#include "Interface/IMesh.h"
#include "Interface/IShaderProgram.h"
#include "Interface/IShader.h"
#include "Maths.h"
#include "RenderComponent.h"

namespace Zephyrus::Assets
{
	class ITexture2D;
}

using Zephyrus::Assets::IMesh;
using Zephyrus::Render::ShaderType;

namespace Zephyrus::ActorComponent
{
	class MeshComponent : public RenderComponent
	{
	protected:
		IMesh* mMesh{ nullptr };
		Vector2D mTiling;
		std::vector<Assets::ITexture2D*> mTextures;
		unsigned int mTextureIndex = 0;
	public:
		MeshComponent(Actor* pOwner);
		virtual ~MeshComponent();

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new MeshComponent(pOwner); }

		void OnStart() override;
		void OnEnd() override;
		std::vector<PropertyDescriptor> GetProperties() override;

		virtual void Draw(const Zephyrus::Render::IRenderer& pRenderer) override;

		virtual void SetMesh(IMesh* pMesh);
		void SetTextureIndex(unsigned int pTextureIndex);

		void AddTexture(Assets::ITexture2D* pTexture);
		Assets::ITexture2D* GetTexture(unsigned int pTextureIndex);
		inline unsigned int GetTextureArraySize() const { return mTextures.size(); }
		inline std::vector<Assets::ITexture2D*> GetAllTextures() const { return mTextures; }

		void SetShaderProgram(Render::IShaderProgram* pShaderProgram);

		void SetTiling(const Vector2D& pTiling);

		inline IMesh* GetMesh() const { return mMesh; }
	};
}
