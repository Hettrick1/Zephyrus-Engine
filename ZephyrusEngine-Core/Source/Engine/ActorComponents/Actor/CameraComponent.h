#pragma once

#include "Quaternion.h"
#include "Matrix4DRow.h"
#include "Component.h"

class Actor;

/**
 * @brief CameraComponent handles the camera logic for an Actor, providing world transform and update functionalities.
 * It works with the Camera Manager.
 */
class CameraComponent : public Component
{
public:
	CameraComponent(Actor* pOwner, int updateOder = 0);
	~CameraComponent();

	static Component* Create(Actor* pOwner) { return new CameraComponent(pOwner); }
	static bool registered;

	// Returns the world transform matrix of the camera
	Matrix4DRow GetWorldTransform();

	// Computes the relative transform of the camera
	void ComputeRelativeTransform();

	void UpdateCam();
};
