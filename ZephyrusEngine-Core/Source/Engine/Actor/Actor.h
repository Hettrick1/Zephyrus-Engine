#pragma once
#include "ActorState.h"
#include "TransformComponent.h"
#include "Bullet/BulletRigidbodyComponent.h"
#include "ISceneContext.h"
#include <vector>
#include <algorithm>
#include <string_view>
#include "JSONUtils.h"

namespace Zephyrus::Scenes
{
	class Scene;
}
namespace Zephyrus::ActorComponent
{
	class Component;
}

using Zephyrus::Scenes::Scene;

namespace Zephyrus::ActorComponent
{
	/**
	 * @brief Represents an entity in the scene that can have components and interact with the world.
	 * The Actor class manages its own state, transform, components, and can be attached to a Scene.
	 */

	class Actor 
	{
	protected:
		ISceneContext* mContext{ nullptr };
		Scene& mScene;
		TransformComponent mTransformComponent;
		BulletRigidbodyComponent* mRigidbody{ nullptr };
		std::vector<Component*> mComponents;
		std::vector<Component*> mPendingComponents;
		std::string mName{ "" };
		std::string mPrefab{ "" };
		std::string mUUID{ "" };
		std::vector<std::string> mTags;
		std::vector<std::string> mComponentsIds;
		float mLod{ 0 };
		ActorState mState;
		bool mIsSelected{ false };
		bool mIsUpdatingComponents{ false };
	public:
		Actor(ISceneContext* pSceneContext, Scene& pScene, Vector3D pPosition = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0), std::string pName = "");
		~Actor();

		virtual void Start();
		virtual void Update();
		virtual void Destroy();

		virtual void Deserialize(Serialization::IDeserializer& pReader);
		virtual void Serialize(Serialization::ISerializer& pWriter);
		virtual void SerializePrefab(const std::string& pFilePath);

		void AttachScene(const Scene& pScene);
		void AddComponent(Component* pComponent);
		void RemoveComponent(Component* pComponent);
		void SetActive(const ActorState& pState);
		void SetPosition(const Vector3D& pPosition);
		void SetSize(const Vector3D& pSize);
		void SetRotation(const Quaternion& pRotation);
		void RotateX(float pAnle);
		void RotateY(float pAngle);
		void RotateZ(float pAngle);

		void SetName(const std::string& pName);
		void SetUUID(const std::string& pUUID);
		void SetPrefab(const std::string& pPrefab);
		void SetRigidBody(BulletRigidbodyComponent* pRigidbody);
		void AddTag(std::string_view  pTag);
		void RemoveTag(std::string_view pTag);
		inline bool HasTag(std::string_view pTag) const
		{
			return std::find(mTags.begin(), mTags.end(), pTag) != mTags.end();
		}

		void AddComponentId(std::string_view  pId);
		void RemoveComponentId(std::string_view pId);
		inline bool HasComponentId(std::string_view pId) const
		{
			return std::find(mComponentsIds.begin(), mComponentsIds.end(), pId) != mComponentsIds.end();
		}
		inline std::vector<std::string> GetComponentsIds() const { return mComponentsIds; }

		inline std::vector<Component*> GetComponents() const { return mComponents; }
		inline ActorState GetState() const { return mState; }
		inline Scene& GetScene() const { return mScene; }
		inline TransformComponent& GetTransformComponent() { return mTransformComponent; }
		inline BulletRigidbodyComponent* GetRigidBody() const { return mRigidbody; }
		inline std::vector<std::string> GetTag() const { return mTags; }
		inline float GetLod() const { return mLod; }
		inline std::string GetUUID() const { return mUUID; }
		inline std::string GetName() const { return mName; }
		inline std::string GetPrefabName() const { return mPrefab; }
		inline Vector3D GetPosition() const { return mTransformComponent.GetPosition(); }
		inline Vector3D GetSize() const { return mTransformComponent.GetSize(); }
		inline Vector3D GetRotationEuler() const { return mTransformComponent.GetRotation().ToEuler(); }
		inline void SetSelected(bool pSelected) { mIsSelected = pSelected; }
		inline bool GetIsSelected() const { return mIsSelected; }
		inline ISceneContext* GetSceneContext() const { return mContext; }

		Component* GetComponentWithId(const std::string pId);

		// Get the first component of a specific type
		template<typename  C>
		C* GetComponentOfType() const
		{
			C* result = nullptr;
			for (Component* component : mComponents)
			{
				result = dynamic_cast<C*>(component);
				if (result != nullptr) return result;
			}
			return nullptr;
		}
		template<typename  C>
		std::vector<C*> GetAllComponentOfType() const
		{
			std::vector<C*> result;
			for (Component* component : mComponents)
			{
				if (auto comp = dynamic_cast<C*>(component))
				{
					result.push_back(comp);
				}
			}
			return result;
		}
	};
}
