#pragma once
#include "Matrix4DRow.h"
#include "Vector3D.h"
#include "rapidjson/document.h"
#include <algorithm>
#include <string>
#include <vector>
#include <string_view>
#include "JSONUtils.h"

class Actor;
/**
 * @brief Base class for all components that can be attached to an Actor.
 * Provides transform management (position, size, rotation) relative to its owner.
 * Components can override lifecycle methods (OnStart, Update, OnEnd).
 */
class Component
{
protected:
	bool mIsActive = true;
	Actor* mOwner = nullptr;
	int mUpdateOrder = 0;
	Matrix4DRow mRelativeTransform = Matrix4DRow::Identity;
	Vector3D mRelativePosition;
	Vector3D mRelativeSize;
	Quaternion mRelativeRotation;
	std::vector<std::string> mComponentTags;
	std::string mComponentName;
	std::string mComponentId;
public:
	Component() = delete;
	Component(Actor* pOwner, std::string pName = "", int pUpdateOder = 0);
	virtual ~Component();

	void SetId(const std::string& pId);
	inline std::string GetId() const { return mComponentId; }

	virtual void OnStart();
	virtual void Update();
	virtual void OnEnd();

	virtual void Deserialize(const rapidjson::Value& pData);
	virtual void BeginSerialize(Serialization::Json::JsonWriter& pWriter);
	virtual void Serialize(Serialization::Json::JsonWriter& pWriter);
	virtual void EndSerialize(Serialization::Json::JsonWriter& pWriter);

	void SetRelativePosition(const Vector3D& pPosition);
	void SetRelativeSize(const Vector3D& pSize);
	void SetRelativeRotation(const Quaternion& pRotation);
	void RelativeRotateX(float pAngle);
	void RelativeRotateY(float pAngle);
	void RelativeRotateZ(float pAngle);

	void AddTag(std::string_view pTag);
	void RemoveTag(std::string_view pTag);
	inline bool HasTag(std::string_view pTag) const
	{
		return std::find(mComponentTags.begin(), mComponentTags.end(), pTag) != mComponentTags.end();
	}

	// Returns the world transform matrix of the component
	virtual Matrix4DRow GetWorldTransform();

	// Computes the relative transform matrix based on position, size, and rotation
	virtual void ComputeRelativeTransform();

	inline Matrix4DRow GetRelativeTransform() const { return mRelativeTransform; }

	inline Vector3D RelativeRight() const
	{
		return Vector3D(-mRelativeTransform.mat[0][0], -mRelativeTransform.mat[1][0], -mRelativeTransform.mat[2][0]);
	}

	inline Vector3D RelativeForward() const
	{
		return Vector3D(mRelativeTransform.mat[0][1], mRelativeTransform.mat[1][1], mRelativeTransform.mat[2][1]);
	}

	inline Vector3D RelativeUp() const
	{
		return Vector3D(mRelativeTransform.mat[0][2], mRelativeTransform.mat[1][2], mRelativeTransform.mat[2][2]);
	}

	inline Vector3D GetRelativePosition() const { return mRelativePosition; }
	inline Vector3D GetRelativeSize() const { return mRelativeSize; }
	Actor* GetOwner() const;
	Vector3D GetWorldPosition() const;

	inline std::string GetName() const { return mComponentName; }
};
