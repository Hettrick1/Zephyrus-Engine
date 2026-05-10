#pragma once

#include "Component.h"
#include "IRenderer.h"
#include <memory>

namespace Zephyrus::ActorComponent
{
	class AiControllerComponent : public Component
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> mImpl;

	public:
		AiControllerComponent(Actor* pOwner);
		virtual ~AiControllerComponent() override;

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new AiControllerComponent(pOwner); }
		virtual std::vector<PropertyDescriptor> GetProperties() override;

		void OnStart() override;
		void Update() override;

		void SetTarget(const Vector3D& target);
		void ResetTarget();
	};
}
