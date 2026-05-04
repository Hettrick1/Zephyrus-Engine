#pragma once

#include "Component.h"
#include "IRenderer.h"
#include <memory>

namespace Zephyrus::ActorComponent
{
	class NavGridVolumeComponent : public Component
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> mImpl;

	public:
		NavGridVolumeComponent(Actor* pOwner);
		virtual ~NavGridVolumeComponent() override;

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new NavGridVolumeComponent(pOwner); }
		virtual std::vector<PropertyDescriptor> GetProperties() override;
		
		void OnStart() override;

		Vector3D GetGridSize() const;
		Vector2D GetAgentSize() const;
		float GetAgentWidth() const;
		float GetAgentHeight() const;
		Vector2D GetNumberOfPoints() const;
		
		bool GetShowLines() const;
		bool GetShowNodePosition() const;
		bool GetShowAgentCollision() const;


	};
}