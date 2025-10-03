#include "PhysicsDebugRenderer.h"
#include "Assets.h"
#include "SceneManager.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::Physics
{
    PhysicsDebugRenderer::PhysicsDebugRenderer()
        : m_debugMode(DBG_DrawWireframe)
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexArray(0);

        mDebugVertex = *AssetsManager::LoadShader("Debug.vert", ShaderType::VERTEX, "DebugVert");
        mDebugFragment = *AssetsManager::LoadShader("Debug.frag", ShaderType::FRAGMENT, "DebugFrag");
        mDebugShaderProgram = *AssetsManager::LoadShaderProgram({ &mDebugVertex, &mDebugFragment }, "debugSP");
        mProj = Matrix4DRow::CreatePerspectiveFOV(70, 1920, 1080, 0.00000001f, 10000000);
    }

    void PhysicsDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
    {
        mLines.push_back(from.x());
        mLines.push_back(from.y());
        mLines.push_back(from.z());

        mLines.push_back(to.x());
        mLines.push_back(to.y());
        mLines.push_back(to.z());
    }

    void PhysicsDebugRenderer::SetProjectionMatrix(const Matrix4DRow& pProj)
    {
        mProj = pProj;
    }

    void PhysicsDebugRenderer::FlushDraw(Zephyrus::ActorComponent::CameraComponent* cam)
    {
        if (mLines.empty()) return;

        glLineWidth(4);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, mLines.size() * sizeof(float), mLines.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(vao);
        mDebugShaderProgram.Use();
        Matrix4DRow mView;
        if (cam == nullptr)
        {
            auto camera = SceneManager::ActiveScene->GetCameraManager()->GetActiveCamera();
            mView = camera->GetViewMatrix();
            mProj = camera->GetProjMatrix();
        }
        else
        {
            mView = cam->GetViewMatrix();
            mProj = cam->GetProjMatrix();
        }
        auto wt = Matrix4DRow::Identity;
        mDebugShaderProgram.setVector3f("uColor", Vector3D(0.0, 1.0, 0));
        mDebugShaderProgram.setMatrix4Row("uViewProj", mView * mProj);
        mDebugShaderProgram.setMatrix4Row("uWorldTransform", wt);
        glDrawArrays(GL_LINES, 0, mLines.size() / 3);
        glBindVertexArray(0);
        glLineWidth(6);

        mLines.clear();
    }
}
