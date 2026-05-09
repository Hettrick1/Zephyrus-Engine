#pragma once

#include "IRenderer.h"
#include "VertexArray.h"
#include "Interface/IShaderProgram.h"
#include "Window.h"
#include "Matrix4DRow.h"
#include "DebugLine.h"

/**
 * @brief Renders debug visualizations such as colliders and lines for debugging purposes.
 */
namespace Zephyrus::Render {
	class DebugRenderer
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> mImpl;
		
	public:
		DebugRenderer();
		~DebugRenderer();

		/// Initializes the debug renderer with the given window.
		void Initialize(const Window& pWindow);
		/// Releases all resources used by the debug renderer.
		void Unload();

		void Draw(IRenderer& pRenderer);

		void AddDebugLine(const Zephyrus::Debug::DebugLine& pLine, int index = -1);
		void AddDebugBox(const Matrix4DRow& pWorldTransform);
		void AddDebugBox(const Zephyrus::Debug::DebugBox& pBox, int index = -1);

		void AddPersistantDebugBox(const Zephyrus::Debug::PersistantDebugBox& pBox);
		void RemovePersistantDebugBox(const Zephyrus::Debug::PersistantDebugBox& pBox);

		void DrawSelectedBox(const Matrix4DRow& pWorldTransform);

		/// Sets the view matrix for debug rendering.
		void SetViewMatrix(const Matrix4DRow& pViewMatrix);
		void SetProjMatrix(const Matrix4DRow& pProjMatrix);

		void FlushDebugElements();
		void FlushDebugLines(int index = -1);
		void FlushDebugBoxes(int index = -1);

		void SetDrawDebug(bool pDraw);
		void SetDrawLines(bool pDraw);
		void SetDrawBoxes(bool pDraw);
		void SetDrawSelected(bool pDraw);
	};
}