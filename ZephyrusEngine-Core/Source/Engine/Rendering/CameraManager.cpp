#include "CameraManager.h"
#include "PrefabFactory.h"
#include "ZPCommons.h"
#include "Log.h"

void CameraManager::AddCamera(CameraComponent* pCam)
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

void CameraManager::RemoveCamera(CameraComponent* pCam)
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

void CameraManager::SetActiveCamera(CameraComponent* pCam)
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

void CameraManager::OnPlay()
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

void CameraManager::OnStop()
{
    mActiveCamera = nullptr;
}

void CameraManager::Unload()
{
    mCameras.clear();
}

 void CameraManager::Update()
{
     if (mActiveCamera)
     {
         mActiveCamera->UpdateMatrices();
     }
}

 void CameraManager::RenderActiveCamera()
 {
     if (mActiveCamera)
     {
         mActiveCamera->UpdateMatrices();
         mActiveCamera->RenderScene();
     }
     else
     {
         auto cameraActor = Zephyrus::Factory::PrefabFactory::SpawnActorFromPrefab("CameraActor");
         mActiveCamera = cameraActor->GetComponentOfType<CameraComponent>();
         mActiveCamera->UpdateMatrices();
         mActiveCamera->RenderScene();
     }
     Zephyrus::Commons::GetCurrentScene()->EndRender();

     /*for (auto* cam : mCameraInRenderTargets)
     {
         if (cam != mActiveCamera)
         {
            cam->UpdateMatrices();
            cam->RenderScene();
         }
     }*/
 }
