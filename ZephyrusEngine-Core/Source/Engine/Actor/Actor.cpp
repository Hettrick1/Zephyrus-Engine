#include "pch.h"
#include "Actor.h"
#include "Component.h"
#include "Scene.h"
#include <algorithm>
#include "ISerializationFactory.h"

#define UUID_SYSTEM_GENERATOR
#include "uuid.h"

namespace Zephyrus::ActorComponent
{
    Actor::Actor(ISceneContext* pSceneContext, Scene& pScene, Vector3D pPosition, Vector3D pSize, Quaternion pRotation, std::string pName) :
        mName(pName), mState(ActorState::Active), mScene(pScene), mContext(pSceneContext), mLod(16), mIsUpdatingComponents(false)
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
        AddComponentId(pComponent->GetId());
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
        RemoveComponentId(pComponent->GetId());
    }

    void Actor::SetActive(const ActorState& pState)
    {
        mState = pState;
        if (mState != ActorState::Active)
        {
            for (auto& comp : mComponents)
            {
                comp->SetActive(false);
            }
            return;
        }
        for (auto& comp : mComponents)
        {
            comp->SetActive(true);
        }
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
        if (mState != ActorState::Active)
        {
            return;
        }
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

    void Actor::Deserialize(Serialization::IDeserializer& pReader)
    {
        if (auto uuid = pReader.ReadString("uuid"))
        {
            SetUUID(*uuid);
        }
        else
        {
            std::string id = uuids::to_string(uuids::uuid_system_generator{}());
            SetUUID(id);
        }
        if (auto name = pReader.ReadString("name"))
        {
            SetName(*name);
        }
        else
        {
            SetName("UnamedActor");
        }

        if (auto state = pReader.ReadString("state"))
        {
            std::string stateStr = *state;
            SetActive(StringToActorState(stateStr));
        }

        if (pReader.BeginObject("transform"))
        {
            if (auto pos = pReader.ReadVector3D("position"))
            {
                SetPosition(*pos);
            }

            if (auto size = pReader.ReadVector3D("size"))
            {
                SetSize(*size);
            }

            if (auto rot = pReader.ReadVector3D("rotation"))
            {
                SetRotation(Quaternion(*rot));
            }
            pReader.EndObject();
        }

        if (auto arr = pReader.ReadArrayString("actorTags"))
        {
            for (auto& element : *arr)
            {
                AddTag(element);
            }
        }
    }

    void Actor::Serialize(Serialization::ISerializer& pWriter)
    {
        pWriter.BeginObject();
        pWriter.WriteString("prefabName", mPrefab);
        pWriter.WriteString("uuid", mUUID);
        pWriter.WriteString("name", mName);
        pWriter.WriteString("state", ActorStateToString(mState));
        if (!mTags.empty())
        {
            pWriter.BeginArray("actorTags");
            for (auto tag : mTags)
            {
                pWriter.PushString(tag);
            }
            pWriter.EndArray();
        }
        pWriter.BeginObject("transform");
        pWriter.WriteVector3D("position", GetPosition());
        pWriter.WriteVector3D("rotation", GetRotationEuler());
        pWriter.WriteVector3D("size", GetSize());
        pWriter.EndObject();

        pWriter.BeginArray("components");
        for (auto& comp : mComponents)
        {
            comp->Serialize(pWriter);
        }
        pWriter.EndArray();
        pWriter.EndObject();
    }

    void Actor::SerializePrefab(const std::string& pFilePath)
    {
        auto writer = mContext->GetSerializationFactory()->CreateSerializer();

        writer->WriteString("name", mName);
        writer->WriteString("state", ActorStateToString(mState));
        if (!mTags.empty())
        {
            writer->BeginArray("actorTags");
            for (auto tag : mTags)
            {
                writer->PushString(tag);
            }
            writer->EndArray();
        }
        writer->BeginObject("transform");
        writer->WriteVector3D("position", GetPosition());
        writer->WriteVector3D("rotation", GetRotationEuler());
        writer->WriteVector3D("size", GetSize());
        writer->EndObject();

        writer->BeginArray("components");
        for (auto& comp : mComponents)
        {
            comp->Serialize(*writer);
        }
        writer->EndArray();
        writer->SaveDocument(pFilePath);
    }

    void Actor::SetName(const std::string& pName)
    {
        mName = pName;
    }

    void Actor::SetUUID(const std::string& pUUID)
    {
        mUUID = pUUID;
    }

    void Actor::SetPrefab(const std::string& pPrefab)
    {
        mPrefab = pPrefab;
    }

    void Actor::SetRigidBody(BulletRigidbodyComponent* pRigidbody)
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

    void Actor::AddTag(std::string_view pTag)
    {
        if (std::find(mTags.begin(), mTags.end(), pTag) == mTags.end())
        {
            mTags.emplace_back(pTag);
        }
    }

    void Actor::RemoveTag(std::string_view  pTag)
    {
        mTags.erase(std::remove(mTags.begin(), mTags.end(), pTag), mTags.end());
    }

    void Actor::AddComponentId(std::string_view pId)
    {
        if (std::find(mComponentsIds.begin(), mComponentsIds.end(), pId) == mComponentsIds.end())
        {
            mComponentsIds.emplace_back(pId);
        }
    }

    void Actor::RemoveComponentId(std::string_view pId)
    {
        mComponentsIds.erase(std::remove(mComponentsIds.begin(), mComponentsIds.end(), pId), mComponentsIds.end());
    }

    Component* Actor::GetComponentWithId(const std::string pId)
    {
        if (pId == "")
        {
            return nullptr;
        }
        for (auto comp : mComponents)
        {
            if (comp->GetId() == pId)
            {
                return comp;
            }
        }
        for (auto comp : mPendingComponents)
        {
            if (comp->GetId() == pId)
            {
                return comp;
            }
        }
        return nullptr;
    }
}