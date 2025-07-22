#include "Actor.h"
#include "Component.h"
#include "Scene.h"
#include "SceneManager.h"
#include <algorithm>

Actor::Actor(Vector3D pPosition, Vector3D pSize, Quaternion pRotation) : 
    mState(ActorState::Active), mScene(*SceneManager::ActiveScene), mTag(""), mRigidbody(nullptr), mLod(16), mIsUpdatingComponents(false)
{
    mTransformComponent.SetPosition(pPosition);
    mTransformComponent.SetSize(pSize);
    mTransformComponent.SetRotation(pRotation);
    mTransformComponent.SetOwner(this);
}

Actor::~Actor()
{
}

void Actor::Start()
{
    for (auto* comp : mComponents) {
        comp->OnStart();
    }
}

void Actor::AttachScene(Scene& pScene)
{
    mScene = pScene;
}

void Actor::AddComponent(Component* pComponent)
{
    if (mIsUpdatingComponents) 
    {
        mPendingComponents.emplace_back(pComponent);
    }
    else 
    {
        mComponents.emplace_back(pComponent);
    }
}

void Actor::RemoveComponent(Component* pComponent)
{
    std::vector<Component*>::iterator it = find(mPendingComponents.begin(), mPendingComponents.end(), pComponent);
    if (it != mPendingComponents.end())
    {
        iter_swap(it, mPendingComponents.end() - 1);
        mPendingComponents.pop_back();
    }
    it = find(mComponents.begin(), mComponents.end(), pComponent);
    if (it != mComponents.end())
    {
        iter_swap(it, mComponents.end() - 1);
        mComponents.pop_back();
    }
}

void Actor::SetActive(ActorState pState)
{
    mState = pState;
}

void Actor::SetPosition(Vector3D pPosition)
{
    mTransformComponent.SetPosition(pPosition);
}

void Actor::SetSize(Vector3D pSize)
{
    mTransformComponent.SetSize(pSize);
}

void Actor::RotateX(float pAnle)
{
    mTransformComponent.RotateX(pAnle);
}

void Actor::RotateY(float pAngle)
{
    mTransformComponent.RotateY(pAngle);
}

void Actor::RotateZ(float pAngle)
{
    mTransformComponent.RotateZ(pAngle);
}

void Actor::Update()
{
    mIsUpdatingComponents = true;
    for (Component* component : mComponents)
    {
        component->Update();
    }
    mIsUpdatingComponents = false;
    for (Component* component : mPendingComponents)
    {
        mComponents.emplace_back(component);
    }
    mPendingComponents.clear();
}

void Actor::Destroy()
{
    for (auto component : mComponents) {
        component->OnEnd();
    }

    for (auto& component : mComponents) {
        if (component != nullptr)
        {
            component = nullptr;
        }
    }
    mComponents.clear();

    for (auto& component : mPendingComponents) {
        component = nullptr;
    }
    mPendingComponents.clear();
}

std::vector<Component*> Actor::GetComponents() const
{
    return mComponents;
}

ActorState Actor::GetState()
{
    return mState;
}

Scene& Actor::GetScene()
{
    return mScene;
}

TransformComponent& Actor::GetTransformComponent()
{
    return mTransformComponent;
}

void Actor::SetRigidBody(RigidbodyComponent* pRigidbody)
{
    if (mRigidbody == nullptr)
    {
        mRigidbody = pRigidbody;
    }
    else
    {
        Log::Error(LogType::System, "You already have a rigidbody attached to this actor !");
    }
}

void Actor::SetTag(std::string pTag)
{
    mTag = pTag;
}

bool Actor::HasTag(std::string pTag)
{
    if (mTag == pTag) 
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Actor::UpdateComponentsTransform()
{

}
