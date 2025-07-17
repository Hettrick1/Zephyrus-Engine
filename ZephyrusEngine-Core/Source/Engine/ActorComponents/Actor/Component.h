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
public:
	Component() = delete;
	Component(Actor* owner, int updateOder = 0);
	virtual ~Component();

	virtual void OnStart();
	virtual void Update();
	virtual void OnEnd();

	void SetRelativePosition(Vector3D pPosition);
	void SetRelativeSize(Vector3D pSize);
	void SetRelativeRotation(Quaternion pRotation);
	void RelativeRotateX(float pAngle);
	void RelativeRotateY(float pAngle);
	void RelativeRotateZ(float pAngle);

	// Returns the world transform matrix of the component
	virtual Matrix4DRow GetWorldTransform();

	// Computes the relative transform matrix based on position, size, and rotation
	virtual void ComputeRelativeTransform();

	Matrix4DRow GetRelativeTransform() const { return mRelativeTransform; }

	Vector3D RelativeRight() const
	{
		return Vector3D(-mRelativeTransform.mat[0][0], -mRelativeTransform.mat[1][0], -mRelativeTransform.mat[2][0]);
	}

	Vector3D RelativeForward() const
	{
		return Vector3D(mRelativeTransform.mat[0][1], mRelativeTransform.mat[1][1], mRelativeTransform.mat[2][1]);
	}

	Vector3D RelativeUp() const
	{
		return Vector3D(mRelativeTransform.mat[0][2], mRelativeTransform.mat[1][2], mRelativeTransform.mat[2][2]);
	}

	Vector3D GetRelativePosition() const
	{
		return mRelativePosition;
	}
	Vector3D GetRelativeSize() const
	{
		return mRelativeSize;
	}

	// Returns the world position of the component
	Vector3D GetWorldPosition();

	Actor* GetOwner();

protected:
	bool mIsActive;
	Actor* mOwner;
	int mUpdateOrder;
	Matrix4DRow mRelativeTransform;
	Vector3D mRelativePosition;
	Vector3D mRelativeSize;
	Quaternion mRelativeRotation;
};
