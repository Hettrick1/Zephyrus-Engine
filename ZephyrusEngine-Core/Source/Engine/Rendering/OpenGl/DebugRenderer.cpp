#include "DebugRenderer.h"
#include "Assets.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::Render {
	DebugRenderer::DebugRenderer()
		: mDebugBoxVbo(0), mDebugBoxVao(0), mDebugLineVbo(0), mDebugLineVao(0)
		, mDrawDebug(false), mDrawLines(true), mDrawBoxes(true)
	{
	}

	DebugRenderer::~DebugRenderer()
	{
		glDeleteBuffers(1, &mDebugBoxVbo);
		glDeleteVertexArrays(1, &mDebugBoxVao);
		glDeleteBuffers(1, &mDebugLineVbo);
		glDeleteVertexArrays(1, &mDebugLineVao);
	}

	void DebugRenderer::Initialize(Window& pWindow)
	{
		mWindow = &pWindow;
		glLineWidth(4);
		mDebugVertex = AssetsManager::LoadShader("Debug.vert", ShaderType::VERTEX, "DebugVert");
		mDebugFragment = AssetsManager::LoadShader("Debug.frag", ShaderType::FRAGMENT, "DebugFrag");
		mDebugShaderProgram = AssetsManager::LoadShaderProgram({ mDebugVertex, mDebugFragment }, "debugSP");
		mView = Matrix4DRow::CreateLookAt(Vector3D(0, 0, 5), Vector3D::unitX, Vector3D::unitZ);
		mProj = Matrix4DRow::CreatePerspectiveFOV(70.0f, pWindow.GetDimensions().x, pWindow.GetDimensions().y, 0.01f, 10000.0f);
		GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,  -0.5f,  0.5f,  0.5f
		};
		glGenVertexArrays(1, &mDebugBoxVao);
		glGenBuffers(1, &mDebugBoxVbo);

		glBindVertexArray(mDebugBoxVao);
		glBindBuffer(GL_ARRAY_BUFFER, mDebugBoxVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Debug line Buffers

		glGenVertexArrays(1, &mDebugLineVao);
		glGenBuffers(1, &mDebugLineVbo);

		glBindVertexArray(mDebugLineVao);
		glBindBuffer(GL_ARRAY_BUFFER, mDebugLineVbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugRenderer::Unload()
	{
		for (auto& line : mLines)
		{
			if (line)
			{
				delete line;
				line = nullptr;
			}
		}
		mLines.clear();
	}

	void DebugRenderer::Draw(IRenderer& pRenderer)
	{
		glEnable(GL_DEPTH_TEST);
		if (mDrawSelected || mDrawDebug)
		{
			mDebugShaderProgram->Use();
			mDebugShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
		}
		if (mDrawDebug) {
			if (mDrawLines)
			{
				for (auto& line : mLines) // DEBUG ONLY
				{
					DrawDebugLine(line->Start, line->End, line->Hit);
				}
				mLines.clear();
			}
		}
		glDisable(GL_DEPTH_TEST);
	}

	void DebugRenderer::AddDebugLine(Zephyrus::Debug::DebugLine* pLine)
	{
		mLines.push_back(pLine);
	}

	void DebugRenderer::RemoveDebugLine(Zephyrus::Debug::DebugLine* pLine)
	{
		std::vector<Zephyrus::Debug::DebugLine*>::iterator dl;
		dl = std::find(mLines.begin(), mLines.end(), pLine);
		mLines.erase(dl);
	}

	void DebugRenderer::DrawDebugBox(const Vector3D& pMin, const Vector3D& pMax, const Matrix4DRow& pWorldTransform)
	{
		glBindVertexArray(mDebugBoxVao);

		mDebugShaderProgram->Use();
		Matrix4DRow wt = pWorldTransform;
		mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
		mDebugShaderProgram->setVector3f("uColor", Vector3D(0, 1, 0));

		glDrawArrays(GL_LINES, 0, 24);
	}

	void DebugRenderer::DrawSelectedBox(const Matrix4DRow& pWorldTransform)
	{
		glDisable(GL_DEPTH_TEST);
		glLineWidth(2);
		glBindVertexArray(mDebugBoxVao);

		mDebugShaderProgram->Use();

		Matrix4DRow wt = pWorldTransform;

		wt = Matrix4DRow::CreateScale(pWorldTransform.GetScale() * 2);
		wt *= Matrix4DRow::CreateFromQuaternion(pWorldTransform.GetRotation());
		wt *= Matrix4DRow::CreateTranslation(pWorldTransform.GetTranslation());

		mDebugShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
		mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
		mDebugShaderProgram->setVector3f("uColor", Vector3D(1.0, 0.7, 0));

		glDrawArrays(GL_LINES, 0, 24);
		glLineWidth(4);
		glEnable(GL_DEPTH_TEST);
	}

	void DebugRenderer::DrawDebugLine(const Vector3D& pStart, const Vector3D& pEnd, const HitResult& pHit)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mDebugLineVbo);
		float lineVertices[] = {
			pStart.x, pStart.y, pStart.z,
			pEnd.x, pEnd.y, pEnd.z
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVertices), lineVertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		mDebugShaderProgram->Use();
		Matrix4DRow worldTransform = Matrix4DRow::Identity;
		mDebugShaderProgram->setMatrix4Row("uWorldTransform", worldTransform);

		if (pHit.HitActor) {
			mDebugShaderProgram->setVector3f("uColor", Vector3D(1, 0, 0));
		}
		else {
			mDebugShaderProgram->setVector3f("uColor", Vector3D(0, 1, 0));
		}

		glBindVertexArray(mDebugLineVao);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);

		if (pHit.HitActor) // draw a box where it was hit
		{
			const float size = 0.1f;

			Matrix4DRow wt = Matrix4DRow::CreateScale(size * 2);
			wt *= Matrix4DRow::CreateTranslation(pHit.HitPoint - size);

			mDebugShaderProgram->Use();
			mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);

			mDebugShaderProgram->setVector3f("uColor", Vector3D(1, 1, 0));

			glBindVertexArray(mDebugBoxVao);
			glDrawArrays(GL_LINES, 0, 24);
		}
	}

	void DebugRenderer::SetViewMatrix(const Matrix4DRow& pViewMatrix)
	{
		mView = pViewMatrix;
	}

	void DebugRenderer::SetProjMatrix(const Matrix4DRow& pProjMatrix)
	{
		mProj = pProjMatrix;
	}

	void DebugRenderer::SetDrawDebug(bool pDraw)
	{
		mDrawDebug = pDraw;
	}

	void DebugRenderer::SetDrawLines(bool pDraw)
	{
		mDrawLines = pDraw;
	}

	void DebugRenderer::SetDrawBoxes(bool pDraw)
	{
		mDrawBoxes = pDraw;
	}

	void DebugRenderer::SetDrawSelected(bool pDraw)
	{
		mDrawSelected = pDraw;
	}
}