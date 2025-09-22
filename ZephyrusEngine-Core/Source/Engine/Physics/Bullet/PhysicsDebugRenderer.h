#pragma once
#include <LinearMath/btIDebugDraw.h>
#include <vector>
#include <glew.h>

#include "Shader.h"
#include "ShaderProgram.h"

class PhysicsDebugRenderer : public btIDebugDraw
{
private :
    int m_debugMode;
    std::vector<float> mLines;
    unsigned int vao, vbo;
    Shader mDebugVertex;
    Shader mDebugFragment;
    ShaderProgram mDebugShaderProgram;
public:
    PhysicsDebugRenderer();

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

    void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override {}
    void reportErrorWarning(const char* warningString) override { printf("Bullet warning: %s\n", warningString); }
    void draw3dText(const btVector3&, const char*) override {}
    void setDebugMode(int debugMode) override { m_debugMode = debugMode; }
    int getDebugMode() const override { return m_debugMode; }

    void FlushDraw();
};