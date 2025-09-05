#pragma once
#include "ActorState.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include <vector>
#include <algorithm>
#include <string_view>

class Component;
class Scene;

/**
 * @brief Represents an entity in the scene that can have components and interact with the world.
 * The Actor class manages its own state, transform, components, and can be attached to a Scene.
 */
class Actor 
{
protected:
	Scene& mScene;
	ActorState mState;
	TransformComponent mTransformComponent;
	RigidbodyComponent* mRigidbody;
	std::vector<Component*> mComponents;
	std::vector<Component*> mPendingComponents;
	bool mIsUpdatingComponents = false;
	std::string mName = "";
	std::vector<std::string> mTags;
	float mLod = 0;

public:
	Actor(Vector3D pPosition = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0), std::string pName = "");
	Actor(std::string pName);
	~Actor();

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;

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

	void SetName(std::string pName);
	void SetRigidBody(RigidbodyComponent* pRigidbody);
	void AddTag(std::string_view  pTag);
	void RemoveTag(std::string_view pTag);
	inline bool HasTag(std::string_view pTag) const
	{
		return std::find(mTags.begin(), mTags.end(), pTag) != mTags.end();
	}

	inline std::vector<Component*> GetComponents() const { return mComponents; }
	inline ActorState GetState() const { return mState; }
	inline Scene& GetScene() const { return mScene; }
	inline TransformComponent& GetTransformComponent() { return mTransformComponent; }
	inline RigidbodyComponent* GetRigidBody() const { return mRigidbody; }
	inline std::vector<std::string> GetTag() const { return mTags; }
	inline float GetLod() const { return mLod; }
	inline std::string GetName() const { return mName; }
	inline Vector3D GetPosition() const { return mTransformComponent.GetPosition(); }
	inline Vector3D GetSize() const { return mTransformComponent.GetSize(); }
	inline Vector3D GetRotationEuler() const { return mTransformComponent.GetRotation().ToEuler(); }

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

	// Update the transform of all components
	void UpdateComponentsTransform();
};
