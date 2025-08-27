#pragma once

#include "Vector3D.h"
#include "Quaternion.h"
#include "Matrix4D.h"
#include "Matrix4DRow.h"
#include "Maths.h"

class Actor;

/**
 * @brief Component that manages the position, size, and rotation of an Actor.
 * Provides transformation utilities and world transform computation.
 */
class TransformComponent
{
private:
	Vector3D mPosition;
	Vector3D mSize;
	Quaternion mRotation;
	Matrix4DRow mWorldTransform = Matrix4DRow::Identity;
	Actor* mOwner = nullptr;
	bool mNeedsToUpdate = false;
	float mRoll = 0, mPitch = 0, mYaw = 0;

public:
	TransformComponent(Actor* pOwner, const Vector3D& pPosition, const Vector3D& pSize, const Quaternion& pRotation);
	TransformComponent();

	void SetPosition(const Vector3D& pNewPosition);
	void Translate(const Vector3D& pTranslation);
	void SetSize(const Vector3D& pNewSize);
	void SetRotation(const Quaternion& pNewRotation);
	void SetOwner(Actor* pOwner);

	// Rotates the component around the X axis
	void RotateX(float pAngle);
	// Rotates the component around the Y axis
	void RotateY(float pAngle);
	// Rotates the component around the Z axis
	void RotateZ(float pAngle);

	float Pitch() const { return mPitch; }
	float Roll() const { return mRoll; }
	float Yaw() const { return mYaw; }
	// Computes the world transform matrix for the component
	void ComputeWorldTransform();

	inline Vector3D GetPosition() const { return mPosition; }
	inline Vector3D GetSize() const { return mSize; }
	inline Quaternion GetRotation() const { return mRotation; }
	inline Matrix4DRow GetWorldTransform() const { return mWorldTransform; }
	inline Vector3D Right() const { return Vector3D(-mRotation.AsMatrixRow().mat[0][0], -mRotation.AsMatrixRow().mat[1][0], -mRotation.AsMatrixRow().mat[2][0]); }
	inline Vector3D Forward() const { return Vector3D(mRotation.AsMatrixRow().mat[0][1], mRotation.AsMatrixRow().mat[1][1], mRotation.AsMatrixRow().mat[2][1]); }
	inline Vector3D Up() const { return Vector3D(mRotation.AsMatrixRow().mat[0][2], mRotation.AsMatrixRow().mat[1][2], mRotation.AsMatrixRow().mat[2][2]); }
};
