#pragma once

#include <vector>

class CameraComponent;

/**
 * @brief Manages all camera components in the scene, allowing to add, remove, and switch between active cameras.
 * Provides a singleton interface for global access.
 */
class CameraManager
{
public:
	static CameraManager& Instance();
	~CameraManager();

	// Unloads all cameras and resets the manager
	void Unload();

	void AddCamera(CameraComponent* pCameraToAdd);
	void RemoveCamera(CameraComponent* pCameraToRemove);

	// Updates the currently active camera
	void UpdateCurrentCamera();

	void SetCurrentCamera(int pIndex);
	void SetCurrentCamera(CameraComponent* pCameraToActivate);

	inline CameraComponent* GetCurrentCamera() const { return mActiveCamera; }
private:
	CameraManager();
	CameraComponent* mActiveCamera;
	std::vector<CameraComponent*> mCameras;
};
