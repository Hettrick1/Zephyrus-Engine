#pragma once
#include "ActorState.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include <vector>

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
	std::string mTag = "";
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
	void RotateX(float pAnle);
	void RotateY(float pAngle);
	void RotateZ(float pAngle);

	void SetName(std::string pName);
	void SetRigidBody(RigidbodyComponent* pRigidbody);
	void SetTag(const std::string& pTag);
	bool HasTag(const std::string& pTag);

	inline std::vector<Component*> GetComponents() const { return mComponents; }
	inline ActorState GetState() const { return mState; }
	inline Scene& GetScene() const { return mScene; }
	inline TransformComponent& GetTransformComponent() { return mTransformComponent; }
	inline RigidbodyComponent* GetRigidBody() const { return mRigidbody; }
	inline std::string GetTag() const { return mTag; }
	inline float GetLod() const { return mLod; }
	inline std::string GetName() const { return mName; }

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

	// Update the transform of all components
	void UpdateComponentsTransform();
};
