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
public:
	Actor(Vector3D position = 0, Vector3D size = 1, Quaternion rotation = Quaternion(0, 0));
	~Actor();

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;

	void AttachScene(Scene& scene);
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);
	void SetActive(ActorState state);
	void SetPosition(Vector3D pPosition);
	void SetSize(Vector3D pSize);
	void RotateX(float pAnle);
	void RotateY(float pAngle);
	void RotateZ(float pAngle);
	std::vector<Component*> GetComponents() const;
	ActorState GetState();
	Scene& GetScene();
	TransformComponent& GetTransformComponent();

	// Get the rigidbody component if it exists
	inline RigidbodyComponent* GetRigidBody() const { return mRigidbody; }

	void SetRigidBody(RigidbodyComponent* pRigidbody);
	void SetTag(std::string pTag);
	inline std::string GetTag() const { return mTag; }
	bool HasTag(std::string tag);

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

	// Get the level of detail value
	inline float GetLod() const { return mLod; }

protected:
	Scene& mScene;
	ActorState mState;
	TransformComponent mTransformComponent;
	RigidbodyComponent* mRigidbody;
	bool mIsUpdatingComponents;
	std::vector<Component*> mComponents;
	std::vector<Component*> mPendingComponents;
	std::string mTag;
	float mLod;
};
