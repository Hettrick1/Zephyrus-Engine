#pragma once
#include "Component.h"
#include "Interface/IMesh.h"
#include "Shader/ShaderProgram.h"
#include "Interface/IShader.h"
#include "Maths.h"

namespace Zephyrus::Assets
{
	class ITexture;
}

using Zephyrus::Assets::IMesh;
using Zephyrus::Render::ShaderProgram;
using Zephyrus::Render::ShaderType;

namespace Zephyrus::ActorComponent
{
	class MeshComponent : public Component
	{
	protected:
		IMesh* mMesh{ nullptr };
		Render::IShader* mVertexShader{ nullptr };
		Render::IShader* mFragmentShader{ nullptr };
		ShaderProgram mShaderProgram;
		Render::IShader* mOutlineVertexShader{ nullptr };
		Render::IShader* mOutlineFragmentShader{ nullptr };
		ShaderProgram mOutlineShaderProgram;
		Vector2D mTiling;
		std::vector<Assets::ITexture*> mTextures;
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

		virtual void Draw(const Matrix4DRow& pViewProj);
		virtual void DrawSelected(const Matrix4DRow& pViewProj);
		virtual void SetMesh(IMesh& pMesh);
		void SetTextureIndex(unsigned int pTextureIndex);

		void AddTexture(Assets::ITexture* pTexture);
		Assets::ITexture* GetTexture(unsigned int pTextureIndex);
		inline unsigned int GetTextureArraySize() const { return mTextures.size(); }
		inline std::vector<Assets::ITexture*> GetAllTextures() const { return mTextures; }

		void SetShaderProgram(const ShaderProgram& pShaderProgram);

		void SetTiling(const Vector2D& pTiling);

		inline IMesh* GetMesh() const { return mMesh; }
		inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }
	};
}
