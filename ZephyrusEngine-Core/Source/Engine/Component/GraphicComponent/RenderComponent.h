#pragma once

#include "Component.h"
#include "Material/IMaterialInstance.h"
#include "Maths.h"
#include "IRenderer.h"

namespace Zephyrus::ActorComponent
{
	class RenderComponent : public Component
	{
	protected:
		Material::IMaterialInstance* mMaterial{ nullptr };
	public:
		RenderComponent(Actor* pOwner, const std::string& pName);
		virtual ~RenderComponent();

		virtual void Deserialize(Serialization::IDeserializer& pReader) override;
		virtual void Serialize(Serialization::ISerializer& pWriter) override;

		virtual void Draw(const Zephyrus::Render::IRenderer& pRenderer) = 0;
		inline Material::IMaterial* GetMaterial() const { return mMaterial->GetBaseMaterial(); }
		inline Material::IMaterialInstance* GetMaterialInstance() const { return mMaterial; }
		void SetMaterial(Material::IMaterial* newMaterial);
	};
}