#pragma once

#include "IRenderer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Window.h"
#include "Matrix4DRow.h"

/**
 * @brief Renders debug visualizations such as colliders and lines for debugging purposes.
 */
class DebugRenderer
{
private:
	Shader mDebugVertex;
	Shader mDebugFragment;
	ShaderProgram mDebugShaderProgram;
	Matrix4DRow mView, mProj;
	Window* mWindow;
	std::vector<ColliderComponent*> mCollider;
	std::vector<DebugLine*> mLines;
	GLuint mDebugBoxVao, mDebugBoxVbo, mDebugLineVao, mDebugLineVbo;
	bool mDrawDebug = false, mDrawLines = false, mDrawBoxes = false, mDrawSelected = false;

public:
	DebugRenderer();
	~DebugRenderer();

	/// Initializes the debug renderer with the given window.
	void Initialize(Window& pWindow);
	/// Releases all resources used by the debug renderer.
	void Unload();

	void Draw(IRenderer& pRenderer);

	void AddDebugCollider(ColliderComponent* pCol);
	void RemoveDebugCollider(ColliderComponent* pCol);
	void AddDebugLine(DebugLine* pLine);
	void RemoveDebugLine(DebugLine* pLine);

	/// Draws a debug box using the given min/max points and world transform.
	void DrawDebugBox(const Vector3D& pMin, const Vector3D& pMax, const Matrix4DRow& pWorldTransform);

	void DrawSelectedBox(const Matrix4DRow& pWorldTransform);

	/// Draws a debug line between two points, with hit information.
	void DrawDebugLine(const Vector3D& pStart, const Vector3D& pEnd, const HitResult& pHit);

	/// Sets the view matrix for debug rendering.
	void SetViewMatrix(const Matrix4DRow& pViewMatrix);

	void SetDrawDebug(bool pDraw);
	void SetDrawLines(bool pDraw);
	void SetDrawBoxes(bool pDraw);
	void SetDrawSelected(bool pDraw);
};
