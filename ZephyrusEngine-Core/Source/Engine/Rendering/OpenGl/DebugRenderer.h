#pragma once

#include "IRenderer.h"
#include "VertexArray.h"
#include "Interface/IShaderProgram.h"
#include "Window.h"
#include "Matrix4DRow.h"
#include "DebugLine.h"
#include "glew.h"

/**
 * @brief Renders debug visualizations such as colliders and lines for debugging purposes.
 */
namespace Zephyrus::Render {
	class DebugRenderer
	{
	private:
		Render::IShader* mDebugVertex{ nullptr };
		Render::IShader* mDebugFragment{ nullptr };
		Render::IShaderProgram* mDebugShaderProgram{ nullptr };
		Matrix4DRow mView, mProj;
		Window* mWindow{ nullptr };
		std::vector<Zephyrus::Debug::DebugLine> mLines;
		std::vector<float> mLinesVertices;
		std::vector<Matrix4DRow> mBoxes;
		GLuint mDebugBoxVao, mDebugBoxVbo, mDebugLineVao, mDebugLineVbo;
		bool mDrawDebug = true, mDrawLines = true, mDrawBoxes = true, mDrawSelected = false;

		/// Draws a debug box using the given min/max points and world transform.
		void DrawDebugBoxes();
		
	public:
		DebugRenderer();
		~DebugRenderer();

		/// Initializes the debug renderer with the given window.
		void Initialize(Window& pWindow);
		/// Releases all resources used by the debug renderer.
		void Unload();

		void Draw(IRenderer& pRenderer);

		void AddDebugLine(Zephyrus::Debug::DebugLine pLine);
		
		void AddDebugBox(const Matrix4DRow& pWorldTransform);
		void RemoveDebugBox(const Matrix4DRow& pWorldTransform);

		void DrawSelectedBox(const Matrix4DRow& pWorldTransform);

		/// Draws a debug line between two points, with hit information.
		void DrawDebugLine(const Vector3D& pStart, const Vector3D& pEnd, const HitResult& pHit);

		/// Sets the view matrix for debug rendering.
		void SetViewMatrix(const Matrix4DRow& pViewMatrix);
		void SetProjMatrix(const Matrix4DRow& pProjMatrix);

		void SetDrawDebug(bool pDraw);
		void SetDrawLines(bool pDraw);
		void SetDrawBoxes(bool pDraw);
		void SetDrawSelected(bool pDraw);
	};
}