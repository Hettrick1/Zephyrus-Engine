#include "pch.h"
#include "DebugRenderer.h"
#include "AssetsManager.h"
#include "EngineContentIds.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::Render {
	DebugRenderer::DebugRenderer()
		: mDebugBoxForMatricesVbo(0), mDebugBoxForMatricesVao(0), mDebugBoxVbo(0), mDebugBoxVao(0), mDebugLineVbo(0), mDebugLineVao(0)
	{
	}

	DebugRenderer::~DebugRenderer()
	{
		glDeleteBuffers(1, &mDebugBoxForMatricesVbo);
		glDeleteVertexArrays(1, &mDebugBoxForMatricesVao);
		glDeleteBuffers(1, &mDebugLineVbo);
		glDeleteVertexArrays(1, &mDebugLineVao);
		glDeleteBuffers(1, &mDebugBoxVbo);
		glDeleteVertexArrays(1, &mDebugBoxVao);
	}

	void DebugRenderer::Initialize(Window& pWindow)
	{
		mWindow = &pWindow;
		glLineWidth(4);
		mDebugVertex = AssetsManager::GetInstance().LoadShader(SH_DEBUG_VERT, ShaderType::VERTEX);
		mDebugFragment = AssetsManager::GetInstance().LoadShader(SH_DEBUG_FRAG, ShaderType::FRAGMENT);
		mDebugShaderProgram = AssetsManager::GetInstance().LoadShaderProgram({ mDebugVertex, mDebugFragment }, "debugSP");
		mView = Matrix4DRow::CreateLookAt(Vector3D(0, 0, 5), Vector3D::unitX, Vector3D::unitZ);
		mProj = Matrix4DRow::CreatePerspectiveFOV(70.0f, pWindow.GetDimensions().x, pWindow.GetDimensions().y, 0.01f, 10000.0f);
		
		// debug box for matrices Buffers
		glGenVertexArrays(1, &mDebugBoxForMatricesVao);
		glGenBuffers(1, &mDebugBoxForMatricesVbo);

		glBindVertexArray(mDebugBoxForMatricesVao);
		glBindBuffer(GL_ARRAY_BUFFER, mDebugBoxForMatricesVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Debug::BoxVertices), Debug::BoxVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Debug line Buffers

		glGenVertexArrays(1, &mDebugLineVao);
		glGenBuffers(1, &mDebugLineVbo);

		glBindVertexArray(mDebugLineVao);
		glBindBuffer(GL_ARRAY_BUFFER, mDebugLineVbo);

		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		// Debug box Buffers

		glGenVertexArrays(1, &mDebugBoxVao);
		glGenBuffers(1, &mDebugBoxVbo);

		glBindVertexArray(mDebugBoxVao);
		glBindBuffer(GL_ARRAY_BUFFER, mDebugBoxVbo);

		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugRenderer::Unload()
	{
		mLines.clear();
		mLinesVertices.clear();
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
			if (mDrawLines && !mLinesVertices.empty())
			{
				if (mNeedRecomputeLinesBuffer)
				{
					glBindVertexArray(mDebugLineVao);
					glBindBuffer(GL_ARRAY_BUFFER, mDebugLineVbo);

					glBufferData(GL_ARRAY_BUFFER, mLinesVertices.size() * sizeof(float), mLinesVertices.data(), GL_DYNAMIC_DRAW);

					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
					glEnableVertexAttribArray(0);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					mNeedRecomputeLinesBuffer = false;
				}
				
				glBindVertexArray(mDebugLineVao);
				mDebugShaderProgram->Use();
        
				auto wt = Matrix4DRow::Identity;
				mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
				mDebugShaderProgram->setVector3f("uColor", Vector3D(1.0f, 0.7f, 0.0f));
				glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mLinesVertices.size() / 3));
				glBindVertexArray(0);
			}
			if (mDrawBoxes)
			{
				DrawDebugBoxesWithMatrices();
				
				if (mBoxes.empty())
				{
					return;
				}
				if (mNeedRecomputeBoxesBuffer)
				{
					glBindVertexArray(mDebugBoxVao);
					glBindBuffer(GL_ARRAY_BUFFER, mDebugBoxVbo);

					glBufferData(GL_ARRAY_BUFFER, mBoxesVertices.size() * sizeof(float), mBoxesVertices.data(), GL_DYNAMIC_DRAW);

					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(0);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					mNeedRecomputeBoxesBuffer = false;
				}

				glBindVertexArray(mDebugBoxVao);
				mDebugShaderProgram->Use();
    
				auto wt = Matrix4DRow::Identity;
				mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
				mDebugShaderProgram->setVector3f("uColor", Vector3D(0.0f, 1.0f, 0.0f));
				glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mBoxesVertices.size() / 3));
				glBindVertexArray(0);
				mBoxesLastFrame = mBoxes;
			}
		}
		glDisable(GL_DEPTH_TEST);
	}

	void DebugRenderer::AddDebugLine(const Zephyrus::Debug::DebugLine& pLine)
	{
		mLines.push_back(pLine);
		mLinesVertices.push_back(pLine.Start.x);
		mLinesVertices.push_back(pLine.Start.y);
		mLinesVertices.push_back(pLine.Start.z);

		mLinesVertices.push_back(pLine.End.x);
		mLinesVertices.push_back(pLine.End.y);
		mLinesVertices.push_back(pLine.End.z);
	}

	void DebugRenderer::AddDebugBox(const Matrix4DRow& pWorldTransform)
	{
		mBoxesWithMatrices.push_back(pWorldTransform);
	}

	void DebugRenderer::AddDebugBox(const Zephyrus::Debug::DebugBox& pBox)
	{
		mBoxes.push_back(pBox);
		auto v = pBox.getBoxVertices();
		mBoxesVertices.insert(mBoxesVertices.end(), v.begin(), v.end());
	}

	void DebugRenderer::DrawDebugBoxesWithMatrices()
	{
		if (mBoxesWithMatrices.empty())
			return;
			
		glBindVertexArray(mDebugBoxForMatricesVao);
		mDebugShaderProgram->Use();
		for (const auto& box : mBoxesWithMatrices)
		{
			Matrix4DRow wt = box;
			mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
			mDebugShaderProgram->setVector3f("uColor", Vector3D(0, 1, 0));

			glDrawArrays(GL_LINES, 0, 24);
		}
	}

	void DebugRenderer::DrawSelectedBox(const Matrix4DRow& pWorldTransform)
	{
		glLineWidth(2);
		glBindVertexArray(mDebugBoxForMatricesVao);

		mDebugShaderProgram->Use();

		Matrix4DRow wt = pWorldTransform;

		wt = Matrix4DRow::CreateScale(pWorldTransform.GetScale() * 2);
		wt *= Matrix4DRow::CreateFromQuaternion(pWorldTransform.GetRotation());
		wt *= Matrix4DRow::CreateTranslation(pWorldTransform.GetTranslation());

		mDebugShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
		mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
		mDebugShaderProgram->setVector3f("uColor", Vector3D(1.0f, 0.7f, 0.0f));

		glDrawArrays(GL_LINES, 0, 24);
		glLineWidth(4);
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

			glBindVertexArray(mDebugBoxForMatricesVao);
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

	void DebugRenderer::FlushDebugElements()
	{
		mBoxes.clear();
		mBoxesVertices.clear();
		mBoxesWithMatrices.clear();
		mLines.clear();
		mLinesVertices.clear();
		mNeedRecomputeBoxesBuffer = true;
		mNeedRecomputeLinesBuffer = true;
	}

	void DebugRenderer::FlushDebugLines()
	{
		mLines.clear();
		mLinesVertices.clear();
		mNeedRecomputeLinesBuffer = true;
	}

	void DebugRenderer::FlushDebugBoxes()
	{
		mBoxes.clear();
		mBoxesVertices.clear();
		mBoxesWithMatrices.clear();
		mNeedRecomputeBoxesBuffer = true;
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