#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Maths.h"

using Zephyrus::Assets::Mesh;
using Zephyrus::Assets::Texture;

namespace Zephyrus::ActorComponent
{
	class MeshComponent : public Component
	{
	protected:
		Mesh* mMesh = nullptr;
		Shader mVertexShader, mFragmentShader;
		ShaderProgram mShaderProgram;
		Shader mOutlineVertexShader, mOutlineFragmentShader;
		ShaderProgram mOutlineShaderProgram;
		Vector2D mTiling;
		std::vector<Texture*> mTextures;
		unsigned int mTextureIndex = 0;
	public:
		MeshComponent(Actor* pOwner);
		virtual ~MeshComponent();

		void Deserialize(const rapidjson::Value& pData) override;
		void Serialize(Serialization::Json::JsonWriter& pWriter) override;

		static Component* Create(Actor* pOwner) { return new MeshComponent(pOwner); }

		void OnStart() override;
		void OnEnd() override;
		std::vector<PropertyDescriptor> GetProperties() override;

		virtual void Draw(const Matrix4DRow& pViewProj);
		virtual void DrawSelected(const Matrix4DRow& pViewProj);
		virtual void SetMesh(Mesh& pMesh);
		void SetTextureIndex(unsigned int pTextureIndex);

		void AddTexture(Texture* pTexture);
		Texture* GetTexture(unsigned int pTextureIndex);
		inline unsigned int GetTextureArraySize() const { return mTextures.size(); }
		inline std::vector<Texture*> GetAllTextures() const { return mTextures; }

		void SetShaderProgram(const ShaderProgram& pShaderProgram);

		void SetTiling(const Vector2D& pTiling);

		inline Mesh* GetMesh() const { return mMesh; }
		inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }
	};
}
