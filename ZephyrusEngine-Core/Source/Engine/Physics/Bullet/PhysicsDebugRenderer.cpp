#include "pch.h"
#include "PhysicsDebugRenderer.h"
#include "AssetsManager.h"
#include "SceneManager.h"
#include "ISceneContext.h"
#include "EngineContentIds.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::Physics
{
    PhysicsDebugRenderer::PhysicsDebugRenderer()
        : mDebugMode{ DBG_DrawWireframe }
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexArray(0);

        mDebugVertex = AssetsManager::GetInstance().LoadShader(SH_DEBUG_VERT, ShaderType::VERTEX);
        mDebugFragment = AssetsManager::GetInstance().LoadShader(SH_DEBUG_FRAG, ShaderType::FRAGMENT);
        mDebugShaderProgram = AssetsManager::GetInstance().LoadShaderProgram({ mDebugVertex, mDebugFragment }, "debugSP");
    }

    void PhysicsDebugRenderer::SetWorld(PhysicWorld* pWorld)
    {
        mWorld = pWorld;
        setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        mWorld->GetWorld()->setDebugDrawer(this);
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

    void PhysicsDebugRenderer::DrawDebug(const Matrix4DRow& view, const Matrix4DRow& projection)
    {
        if (!mWorld) return;
        glEnable(GL_DEPTH_TEST);
        mWorld->GetWorld()->debugDrawWorld();
        FlushDraw(view, projection);
        glDisable(GL_DEPTH_TEST);
    }

    void PhysicsDebugRenderer::FlushDraw(const Matrix4DRow& view, const Matrix4DRow& projection)
    {
        if (mLines.empty() || !mWorld) return;

        glLineWidth(3);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, mLines.size() * sizeof(float), mLines.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(vao);
        mDebugShaderProgram->Use();
        
        auto wt = Matrix4DRow::Identity;
        mDebugShaderProgram->setVector3f("uColor", Vector3D(0.0, 1.0, 0));
        mDebugShaderProgram->setMatrix4Row("uViewProj", view * projection);
        mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mLines.size() / 3));
        glBindVertexArray(0);
        glLineWidth(6);

        mLines.clear();
    }
}
