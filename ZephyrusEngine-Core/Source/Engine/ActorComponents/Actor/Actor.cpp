#include "Actor.h"
#include "Component.h"
#include "Scene.h"
#include "SceneManager.h"
#include <algorithm>

Actor::Actor(Vector3D pPosition, Vector3D pSize, Quaternion pRotation, std::string pName) :
    mName(pName), mState(ActorState::Active), mScene(*SceneManager::ActiveScene), mTag(""), mRigidbody(nullptr), mLod(16), mIsUpdatingComponents(false)
{
    mTransformComponent.SetPosition(pPosition);
    mTransformComponent.SetSize(pSize);
    mTransformComponent.SetRotation(pRotation);
    mTransformComponent.SetOwner(this);
}

Actor::Actor(std::string pName)
    : mName(pName), mState(ActorState::Active), mScene(*SceneManager::ActiveScene), mTag(""), mRigidbody(nullptr), mLod(16), mIsUpdatingComponents(false)
{
    mTransformComponent.SetPosition(0);
    mTransformComponent.SetSize(1);
    mTransformComponent.SetRotation(Quaternion(0, 0));
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

void Actor::AttachScene(const Scene& pScene)
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

void Actor::SetActive(const ActorState& pState)
{
    mState = pState;
}

void Actor::SetPosition(const Vector3D& pPosition) 
{
    mTransformComponent.SetPosition(pPosition);
}

void Actor::SetSize(const Vector3D& pSize)
{
    mTransformComponent.SetSize(pSize);
}

void Actor::SetRotation(const Quaternion& pRotation)
{
    mTransformComponent.SetRotation(pRotation);
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
    if (mRigidbody != nullptr)
    {
        mRigidbody = nullptr;
    }
    for (auto component : mComponents) {
        component->OnEnd();
    }
    for (auto component : mPendingComponents) {
        component->OnEnd();
    }
    for (auto& component : mComponents) {
        if (component != nullptr)
        {
            delete component;
            component = nullptr;
        }
    }
    mComponents.clear();

    for (auto& component : mPendingComponents) {
        delete component;
        component = nullptr;
    }
    mPendingComponents.clear();
}

void Actor::SetName(std::string pName)
{
    mName = pName;
}

void Actor::SetRigidBody(RigidbodyComponent* pRigidbody)
{
    if (mRigidbody == nullptr)
    {
        mRigidbody = pRigidbody;
    }
    else
    {
        ZP_WARN("You already have a rigidbody attached to this actor !");
    }
}

void Actor::SetTag(const std::string& pTag)
{
    mTag = pTag;
}

bool Actor::HasTag(const std::string& pTag)
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