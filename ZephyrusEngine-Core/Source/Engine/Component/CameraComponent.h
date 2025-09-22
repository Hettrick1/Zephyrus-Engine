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
	Matrix4DRow mViewMatrix;

	CameraComponent(Actor* pOwner, int updateOder = 0);
	~CameraComponent();

	void Deserialize(const rapidjson::Value& pData) override;
	void Serialize(Serialization::Json::JsonWriter& pWriter) override;

	void OnStart() override;
	void OnEnd() override;

	static Component* Create(Actor* pOwner) { return new CameraComponent(pOwner); }

	// Returns the world transform matrix of the camera
	Matrix4DRow GetWorldTransform();

	// Computes the relative transform of the camera
	void ComputeRelativeTransform();

	void UpdateCam();
};
