#include "NewCameraComponent.h"
#include "SceneManager.h"
#include <glew.h>

NewCameraComponent::NewCameraComponent(Actor* pOwner, int pWidth, int pHeight, CameraUsage pUsage)
    : Component(pOwner), usage(pUsage), mWidth(pWidth), mHeight(pHeight)
{
    renderTarget = new RenderTarget(pWidth, pHeight);
}

NewCameraComponent::~NewCameraComponent()
{
    if (renderTarget) {
        delete renderTarget;
        renderTarget = nullptr;
    }
}

void NewCameraComponent::UpdateMatrices()
{
    if (!mOwner) {
        std::cerr << "NewCameraComponent::UpdateMatrices: Owner actor is null!" << std::endl;
        return;
    }

    Vector3D position = GetWorldTransform().GetTranslation();
    Vector3D forward = GetWorldTransform().GetYAxis();
    Vector3D up = Vector3D::unitZ;

    mViewMatrix = Matrix4DRow::CreateLookAt(position, position + forward, up);

    mProjMatrix = Matrix4DRow::CreatePerspectiveFOV(mFov, mWidth, mHeight, mNearClip, mFarClip);
}

void NewCameraComponent::RenderScene()
{
    if (!renderTarget) return;

    renderTarget->Bind();
    glViewport(0, 0, renderTarget->GetDimensions().x, renderTarget->GetDimensions().y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    SceneManager::ActiveScene->RenderCurrentSceneOnly();
    // Appeler ici ton renderer pour dessiner la scène
    // Exemple :
    // SceneRenderer::DrawScene(*this);

    RenderTarget::Unbind();
}