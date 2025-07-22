#pragma once

#include "IRenderer.h"
#include "VertexArray.h"
#include "Shaders/ShaderProgram.h"
#include "Window.h"
#include "Matrix4DRow.h"

/**
 * @brief Renders debug visualizations such as colliders and lines for debugging purposes.
 */
class DebugRenderer
{
public:
	DebugRenderer();
	~DebugRenderer();

	/// Initializes the debug renderer with the given window.
	void Initialize(Window& pWindow);
	/// Releases all resources used by the debug renderer.
	void Unload();

	void Draw(IRenderer& pRenderer);

	void AddDebugCollider(ColliderComponent* pCol);
	void AddDebugLine(DebugLine* pLine);

	/// Draws a debug box using the given min/max points and world transform.
	void DrawDebugBox(Vector3D& pMin, Vector3D& pMax, Matrix4DRow pWorldTransform);
	/// Draws a debug line between two points, with hit information.
	void DrawDebugLine(const Vector3D& pStart, const Vector3D& pEnd, const HitResult& pHit);

	/// Sets the view matrix for debug rendering.
	void SetViewMatrix(Matrix4DRow pViewMatrix);

	void SetDrawDebug(bool pDraw);
	void SetDrawLines(bool pDraw);
	void SetDrawBoxes(bool pDraw);
private:
	Shader mDebugVertex;
	Shader mDebugFragment;
	ShaderProgram mDebugShaderProgram;
	Matrix4DRow mView, mProj;
	std::vector<ColliderComponent*> mCollider;
	std::vector<DebugLine*> mLines;
	GLuint mDebugBoxVao, mDebugBoxVbo, mDebugLineVao, mDebugLineVbo;
	bool mDrawDebug = false, mDrawLines = false, mDrawBoxes = false;
};
