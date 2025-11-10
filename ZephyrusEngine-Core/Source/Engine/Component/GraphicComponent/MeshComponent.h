#pragma once
#include "Interface/IMesh.h"
#include "Maths.h"
#include "RenderComponent.h"

namespace Zephyrus::Assets
{
	class ITexture2D;
}

using Zephyrus::Assets::IMesh;

namespace Zephyrus::ActorComponent
{
	class MeshComponent : public RenderComponent
	{
	protected:
		IMesh* mMesh{ nullptr };
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

		inline IMesh* GetMesh() const { return mMesh; }
	};
}
