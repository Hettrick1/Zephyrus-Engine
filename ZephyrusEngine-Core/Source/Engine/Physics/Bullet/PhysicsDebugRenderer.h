#pragma once
#include <LinearMath/btIDebugDraw.h>
#include <vector>
#include <glew.h>

#include "Interface/IShader.h"
#include "Interface/IShaderProgram.h"
#include "CameraComponent.h"

class ISceneContext;

using Zephyrus::Render::ShaderType;

namespace Zephyrus::Physics
{
    class PhysicsDebugRenderer : public btIDebugDraw
    {
    private:
        int mDebugMode;
        std::vector<float> mLines;
        unsigned int vao, vbo;
        Render::IShader* mDebugVertex{ nullptr };
        Render::IShader* mDebugFragment{ nullptr };
        Render::IShaderProgram* mDebugShaderProgram{ nullptr };
        Matrix4DRow mProj;
        ISceneContext* mContext{ nullptr };
    public:
        PhysicsDebugRenderer(ISceneContext* pContext);

        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

        void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override {}
        void reportErrorWarning(const char* warningString) override { printf("Bullet warning: %s\n", warningString); }
        void draw3dText(const btVector3&, const char*) override {}
        void setDebugMode(int debugMode) override { mDebugMode = debugMode; }
        int getDebugMode() const override { return mDebugMode; }

        void SetProjectionMatrix(const Matrix4DRow& pProj);

        void FlushDraw(Zephyrus::ActorComponent::CameraComponent* cam = nullptr);
    };
}