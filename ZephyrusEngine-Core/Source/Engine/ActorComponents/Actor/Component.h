#pragma once
#include "Matrix4DRow.h"
#include "Vector3D.h"

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

public:
	Component() = delete;
	Component(Actor* pOwner, int pUpdateOder = 0);
	virtual ~Component();

	virtual void OnStart();
	virtual void Update();
	virtual void OnEnd();

	void SetRelativePosition(const Vector3D& pPosition);
	void SetRelativeSize(const Vector3D& pSize);
	void SetRelativeRotation(const Quaternion& pRotation);
	void RelativeRotateX(float pAngle);
	void RelativeRotateY(float pAngle);
	void RelativeRotateZ(float pAngle);

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
};
