#include "pch.h"
#include "Component.h"
#include "Actor.h"
#include "Log.h"

namespace Zephyrus::ActorComponent
{
    Component::Component(Actor* pOwner, const std::string& pName, int pUpdateOder)
        : mOwner(pOwner), mUpdateOrder(pUpdateOder), mRelativePosition(0),
        mRelativeRotation(Quaternion(0, 0, 0, 1)), mRelativeSize(1), mComponentName(pName)
    {
        if (mOwner->GetState() != ActorState::Active)
        {
            mIsActive = false;
        }
    }

    Component::~Component()
    {
        mOwner = nullptr;
    }

    void Component::SetId(const std::string& pId)
    {
        mComponentId = pId;
    }

    void Component::OnStart()
    {
    }

    void Component::Update()
    {
    }

    void Component::OnEnd()
    {
    }

    std::vector<PropertyDescriptor> Component::GetProperties()
    {
        return std::vector<PropertyDescriptor>();
    }

    void Component::Deserialize(Serialization::IDeserializer& pReader)
    {
        if (auto pos = pReader.ReadVector3D("relativePosition"))
        {
            SetRelativePosition(*pos);
        }

        if (auto size = pReader.ReadVector3D("relativeSize"))
        {
            SetRelativeSize(*size);
        }

        if (auto rot = pReader.ReadVector3D("relativeRotation"))
        {
            SetRelativeRotation(Quaternion(*rot));
        }

        if (auto arr = pReader.ReadArrayString("tags"))
        {
            for (auto& element : *arr)
            {
                AddTag(element);
            }
        }
    }

    void Component::BeginSerialize(Serialization::ISerializer& pWriter)
    {
        pWriter.BeginObject();
        pWriter.WriteString("type", mComponentName);
        pWriter.WriteString("componentId", mComponentId);
        pWriter.BeginObject("properties");
        pWriter.WriteVector3D("relativePosition", mRelativePosition);
        pWriter.WriteVector3D("relativeSize", mRelativeSize);
        pWriter.WriteVector3D("relativeRotation", mRelativeRotation.ToEuler());
        if (!mComponentTags.empty())
        {
            pWriter.BeginArray("tags");
            for (auto& tag : mComponentTags)
            {
                pWriter.PushString(tag);
            }
            pWriter.EndArray();
        }
    }

    void Component::Serialize(Serialization::ISerializer& pWriter)
    {
        BeginSerialize(pWriter);
        EndSerialize(pWriter);
    }

    void Component::EndSerialize(Serialization::ISerializer& pWriter)
    {
        pWriter.EndObject();
        pWriter.EndObject();
    }

    void Component::SetRelativePosition(const Vector3D& pPosition)
    {
        mRelativePosition = pPosition;
        ComputeRelativeTransform();
    }

    void Component::SetRelativeSize(const Vector3D& pSize)
    {
        mRelativeSize = pSize;
        ComputeRelativeTransform();
    }

    void Component::SetRelativeRotation(const Quaternion& pRotation)
    {
        mRelativeRotation = pRotation;
        ComputeRelativeTransform();
    }

    void Component::RelativeRotateX(float pAngle)
    {
        float piAngle = zpMaths::ToRad(pAngle);
        Quaternion newX(Vector3D::unitX, piAngle);
        mRelativeRotation = Quaternion::Concatenate(newX, mRelativeRotation);
        ComputeRelativeTransform();
    }

    void Component::RelativeRotateY(float pAngle)
    {
        float piAngle = zpMaths::ToRad(pAngle);
        Quaternion newX(Vector3D::unitY, piAngle);
        mRelativeRotation = Quaternion::Concatenate(newX, mRelativeRotation);
        ComputeRelativeTransform();
    }

    void Component::RelativeRotateZ(float pAngle)
    {
        float piAngle = zpMaths::ToRad(pAngle);
        Quaternion newX(Vector3D::unitZ, piAngle);
        mRelativeRotation = Quaternion::Concatenate(newX, mRelativeRotation);
        ComputeRelativeTransform();
    }

    void Component::AddTag(std::string_view pTag)
    {
        if (std::find(mComponentTags.begin(), mComponentTags.end(), pTag) == mComponentTags.end())
        {
            mComponentTags.emplace_back(pTag);
        }
    }

    void Component::RemoveTag(std::string_view pTag)
    {
        mComponentTags.erase(std::remove(mComponentTags.begin(), mComponentTags.end(), pTag), mComponentTags.end());
    }

    Matrix4DRow Component::GetWorldTransform()
    {
        ComputeRelativeTransform();
        if (mParent)
        {
            return  mRelativeTransform * mParent->GetWorldTransform();
        }
        if (mOwner)
        {
            return mRelativeTransform * mOwner->GetTransformComponent().GetWorldTransform();
        }
        return mRelativeTransform;
    }

    void Component::ComputeRelativeTransform()
    {
        if (mOwner)
        {
            mRelativeTransform = Matrix4DRow::CreateScale(mRelativeSize);
            mRelativeTransform *= Matrix4DRow::CreateFromQuaternion(mRelativeRotation);
            mRelativeTransform *= Matrix4DRow::CreateTranslation(mRelativePosition);
        }
        else
        {
            mRelativeTransform = Matrix4DRow::Identity;
        }
    }

    Actor* Component::GetOwner() const
    {
        return mOwner;
    }

    Vector3D Component::GetWorldPosition() const
    {
        return mOwner->GetTransformComponent().GetPosition() + mRelativePosition;
    }
}
