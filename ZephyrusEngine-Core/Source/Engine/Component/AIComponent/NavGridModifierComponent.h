#pragma once

#include "Component.h"
#include "IRenderer.h"
#include <memory>

namespace Zephyrus::ActorComponent
{
	class NavGridModifierComponent : public Component
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> mImpl;

	public:
		NavGridModifierComponent(Actor* pOwner);
		virtual ~NavGridModifierComponent() override;

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new NavGridModifierComponent(pOwner); }
		virtual std::vector<PropertyDescriptor> GetProperties() override;

		void OnStart() override;

		bool IsPointInsideVolume(const Vector3D& point) const;
		float GetWeight() const;
	};
}