#pragma once

#include "Component.h"
#include "IRenderer.h"

namespace Zephyrus::ActorComponent
{
	class NavGridVolumeComponent : public Component
	{
	private:
		Vector2D mGridSize{ 20.0f, 20.0f };
		float mAgentWidth = 0.8f;
		float mAgentHeight = 1.5f;
		bool mShowLines = true;
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
		
		Vector2D GetGridSize() const { return mGridSize; }
		Vector2D GetAgentSize() const { return Vector2D{ mAgentWidth, mAgentHeight }; }
		float GetAgentWidth() const { return mAgentWidth; }
		float GetAgentHeight() const { return mAgentHeight; }
		
		bool GetShowLines() const { return mShowLines; }
	};
}