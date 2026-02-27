#pragma once
#include "Component.h"
#include "MeshComponent.h"
#include "Interface/ICubeMapTexture.h"
#include "Interface/IMesh.h"
#include "Interface/IShaderProgram.h"
#include "Maths.h"

namespace Zephyrus::ActorComponent
{
	class CubeMapMeshComponent : public MeshComponent
	{
	public:
		CubeMapMeshComponent(Actor* pOwner);
		virtual ~CubeMapMeshComponent() override;

		virtual std::vector<PropertyDescriptor> GetProperties() override;
		
		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new MeshComponent(pOwner); }
		
		void Draw(const Zephyrus::Render::IRenderer& pRenderer) override;

	protected:
		Assets::ICubeMapTexture* mCubeMapTexture{nullptr};
	};
}