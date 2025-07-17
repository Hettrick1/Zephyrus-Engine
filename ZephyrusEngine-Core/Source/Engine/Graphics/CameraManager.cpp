#include "CameraManager.h"
#include "CameraComponent.h"
#include "Log.h"

CameraManager& CameraManager::Instance()
{
	static CameraManager instance;
	return instance;
}

CameraManager::CameraManager()
	:mActiveCamera(nullptr)
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::Unload()
{
	mActiveCamera = nullptr;
	mCameras.clear();
}

void CameraManager::AddCamera(CameraComponent* pCameraToAdd)
{
	mCameras.push_back(pCameraToAdd);
	if (mActiveCamera == nullptr)
	{
		mActiveCamera = pCameraToAdd;
	}
}

void CameraManager::RemoveCamera(CameraComponent* pCameraToRemove)
{
	std::vector<CameraComponent*>::iterator sc;
	sc = std::find(mCameras.begin(), mCameras.end(), pCameraToRemove);
	mCameras.erase(sc);
}

void CameraManager::UpdateCurrentCamera()
{
	mActiveCamera->UpdateCam();
}

void CameraManager::SetCurrentCamera(int pIndex)
{
	if (pIndex < static_cast<int>(mCameras.size()))
	{
		mActiveCamera = mCameras[pIndex];
	}
	else 
	{
		Log::Error(LogType::Render, "Camera index out of bounds");
	}
}

void CameraManager::SetCurrentCamera(CameraComponent* pCameraToActivate)
{
	std::vector<CameraComponent*>::iterator sc;
	sc = std::find(mCameras.begin(), mCameras.end(), pCameraToActivate);
	if (sc != mCameras.end())
	{
		mActiveCamera = *sc;
	}
	else
	{
		Log::Error(LogType::Render, "Cameranot found in the camera vector");
	}
}
