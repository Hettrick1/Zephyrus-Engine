#pragma once
#include "Quaternion.h"
#include "Matrix4DRow.h"
#include "Component.h"

namespace Zephyrus::ActorComponent
{
	/**
	 * @brief CameraComponent handles the camera logic for an Actor, providing world transform and update functionalities.
	 * It works with the Camera Manager.
	 */
	class PlayerStartComponent : public Component
	{
	private:
		std::string mPlayerPrefabId;
	public:
		PlayerStartComponent(Actor* pOwner, int updateOder = 0);
		~PlayerStartComponent() override;

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		void OnEnd() override;

		std::vector<PropertyDescriptor> GetProperties() override;

		static Component* Create(Actor* pOwner) { return new PlayerStartComponent(pOwner); }

		inline void SetPlayerPrefabName(const std::string& pPlayerPrefabId) { mPlayerPrefabId = pPlayerPrefabId; }
		inline std::string GetPlayerPrefabName() const
		{
			return mPlayerPrefabId;
		}
	};
}
