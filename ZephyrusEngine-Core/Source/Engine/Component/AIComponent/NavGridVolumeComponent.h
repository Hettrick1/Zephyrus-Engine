#pragma once

#include "Component.h"
#include "IRenderer.h"

namespace Zephyrus::ActorComponent
{
	class NavGridVolumeComponent : public Component
	{
	private:
		Vector2D mGridSize{ 100.0f, 100.0f };
		float mAgentWidth = 0.8f;
		float mAgentHeight = 1.5f;
	public:
		NavGridVolumeComponent(Actor* pOwner);
		virtual ~NavGridVolumeComponent() override;

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new NavGridVolumeComponent(pOwner); }
		virtual std::vector<PropertyDescriptor> GetProperties() override;

		void OnStart() override;
		void Update() override;
		void OnEnd() override;
	};
}