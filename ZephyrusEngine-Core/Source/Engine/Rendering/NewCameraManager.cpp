#include "NewCameraManager.h"
#include "Log.h"

void NewCameraManager::AddCamera(NewCameraComponent* pCam)
{
    if (!pCam) return;

    if (std::find(mCameras.begin(), mCameras.end(), pCam) == mCameras.end())
    {
        mCameras.push_back(pCam);
        if (pCam->usage == CameraUsage::Game)
        {
            mActiveCamera = pCam;
        }
    }
}

void NewCameraManager::RemoveCamera(NewCameraComponent* pCam)
{
    auto it = std::find(mCameras.begin(), mCameras.end(), pCam);
    if (it != mCameras.end())
    {
        if (mActiveCamera == *it) 
        {
            mActiveCamera = nullptr;
        }
        mCameras.erase(it);
    }
}

void NewCameraManager::SetActiveCamera(NewCameraComponent* pCam)
{
    if (!pCam) return;
    if (std::find(mCameras.begin(), mCameras.end(), pCam) != mCameras.end())
    {
        mActiveCamera = pCam;
    }
    else
    {
        ZP_CORE_ERROR("NewCameraManager::SetActiveCamera -> camera not registered!");
    }
}

void NewCameraManager::OnPlay()
{
    if (!mActiveCamera)
    {
        for (auto* cam : mCameras)
        {
            if (cam->usage == CameraUsage::Game)
            {
                mActiveCamera = cam;
                break;
            }
        }
    }
}

void NewCameraManager::OnStop()
{
    mActiveCamera = nullptr;
}

void NewCameraManager::Unload()
{
    mCameras.clear();
}

 void NewCameraManager::Update()
{
     if (mActiveCamera)
     {
         mActiveCamera->UpdateMatrices();
     }
}

 void NewCameraManager::RenderActiveCamera()
 {
     if (mActiveCamera)
     {
         mActiveCamera->UpdateMatrices();
         mActiveCamera->RenderScene();
     }

     /*for (auto* cam : mCameras)
     {
         if (cam != mActiveCamera)
         {
            cam->UpdateMatrices();
         }
     }*/
 }
