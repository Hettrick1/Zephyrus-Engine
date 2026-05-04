#include "pch.h"
#include "DebugRenderer.h"
#include "AssetsManager.h"
#include "EngineContentIds.h"
#include "glew.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::Render {

	struct DebugRenderer::Impl 
	{
		Render::IShader* mDebugVertex{ nullptr };
		Render::IShader* mDebugVertexColorVertex{ nullptr };
		Render::IShader* mDebugFragment{ nullptr };
		Render::IShaderProgram* mDebugShaderProgram{ nullptr };
		Render::IShaderProgram* mDebugVertexColorShaderProgram{ nullptr };
		Matrix4DRow mView, mProj;
		std::vector<Zephyrus::Debug::DebugLine> mLines;
		std::vector<float> mLinesVertices;
		std::vector<Zephyrus::Debug::DebugBox> mBoxes;
		std::vector<float> mBoxesVertices;
		std::vector<Matrix4DRow> mBoxesWithMatrices;
		std::vector<Matrix4DRow> mPersistantBoxes;
		unsigned mDebugBoxForMatricesVao, mDebugBoxForMatricesVbo, mDebugBoxVao, mDebugBoxVbo, mDebugLineVao, mDebugLineVbo;

		bool mDrawDebug = true, mDrawLines = true, mDrawBoxes = true, mDrawSelected = false, mDrawPersistantDebug = true;

		bool mNeedRecomputeBoxesBuffer = true;
		bool mNeedRecomputeLinesBuffer = true;

		void Initialize(const Window& pWindow);
		void DeleteBuffers();
		void Unload();

		void FlushDebugElements();
		void FlushDebugLines();
		void FlushDebugBoxes();

		void SetUniforms(const Matrix4DRow& pWorldTransform, const Vector3D& color);

		/// Draws a debug box using the given min/max points and world transform.
		void DrawDebugBoxesWithMatrices();
		void DrawDebugBoxes();
		void DrawDebugLines();

		void DrawDebug();
	};

	void DebugRenderer::Impl::Initialize(const Window& pWindow)
	{
		glLineWidth(4);
		mDebugVertex = AssetsManager::GetInstance().LoadShader(SH_DEBUG_VERT, ShaderType::VERTEX);
		mDebugVertexColorVertex = AssetsManager::GetInstance().LoadShader(SH_DEBUG_VERTEX_COLOR_VERT, ShaderType::VERTEX);
		mDebugFragment = AssetsManager::GetInstance().LoadShader(SH_DEBUG_FRAG, ShaderType::FRAGMENT);
		mDebugShaderProgram = AssetsManager::GetInstance().LoadShaderProgram({ mDebugVertex, mDebugFragment }, "debugSP");
		mDebugVertexColorShaderProgram = AssetsManager::GetInstance().LoadShaderProgram({ mDebugVertexColorVertex, mDebugFragment }, "debugVertexColorSP");
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

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Debug box Buffers

		glGenVertexArrays(1, &mDebugBoxVao);
		glGenBuffers(1, &mDebugBoxVbo);

		glBindVertexArray(mDebugBoxVao);
		glBindBuffer(GL_ARRAY_BUFFER, mDebugBoxVbo);

		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugRenderer::Impl::DeleteBuffers()
	{
		glDeleteBuffers(1, &mDebugBoxForMatricesVbo);
		glDeleteVertexArrays(1, &mDebugBoxForMatricesVao);
		glDeleteBuffers(1, &mDebugLineVbo);
		glDeleteVertexArrays(1, &mDebugLineVao);
		glDeleteBuffers(1, &mDebugBoxVbo);
		glDeleteVertexArrays(1, &mDebugBoxVao);
	}

	void DebugRenderer::Impl::Unload()
	{
		mLines.clear();
		mLinesVertices.clear();
		mBoxes.clear();
		mBoxesVertices.clear();
		mBoxesWithMatrices.clear();
		mPersistantBoxes.clear();
	}

	void DebugRenderer::Impl::FlushDebugElements()
	{
		mBoxes.clear();
		mBoxesVertices.clear();
		mBoxesWithMatrices.clear();
		mLines.clear();
		mLinesVertices.clear();
		mNeedRecomputeBoxesBuffer = true;
		mNeedRecomputeLinesBuffer = true;
	}

	void DebugRenderer::Impl::FlushDebugLines()
	{
		mLines.clear();
		mLinesVertices.clear();
		mNeedRecomputeLinesBuffer = true;
	}

	void DebugRenderer::Impl::FlushDebugBoxes()
	{
		mBoxes.clear();
		mBoxesVertices.clear();
		mBoxesWithMatrices.clear();
		mNeedRecomputeBoxesBuffer = true;
	}

	void DebugRenderer::Impl::SetUniforms(const Matrix4DRow& worldTransform, const Vector3D& color)
	{
		mDebugShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
		mDebugShaderProgram->setMatrix4Row("uWorldTransform", worldTransform);
		mDebugShaderProgram->setVector3f("uColor", Vector3D(1.0f, 0.7f, 0.0f));
	}

	void DebugRenderer::Impl::DrawDebugBoxes()
	{
		if (mBoxes.empty())
		{
			return;
		}
		if (mNeedRecomputeBoxesBuffer)
		{
			glBindVertexArray(mDebugBoxVao);
			glBindBuffer(GL_ARRAY_BUFFER, mDebugBoxVbo);

			glBufferData(GL_ARRAY_BUFFER, mBoxesVertices.size() * sizeof(float), mBoxesVertices.data(), GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			mNeedRecomputeBoxesBuffer = false;
		}

		glBindVertexArray(mDebugBoxVao);
		mDebugVertexColorShaderProgram->Use();

		auto wt = Matrix4DRow::Identity;
		mDebugVertexColorShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
		mDebugVertexColorShaderProgram->setMatrix4Row("uWorldTransform", wt);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mBoxesVertices.size() / 6));
		glBindVertexArray(0);
	}

	void DebugRenderer::Impl::DrawDebugLines()
	{
		if (mNeedRecomputeLinesBuffer)
		{
			glBindVertexArray(mDebugLineVao);
			glBindBuffer(GL_ARRAY_BUFFER, mDebugLineVbo);

			glBufferData(GL_ARRAY_BUFFER, mLinesVertices.size() * sizeof(float), mLinesVertices.data(), GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			mNeedRecomputeLinesBuffer = false;
		}

		glBindVertexArray(mDebugLineVao);
		mDebugVertexColorShaderProgram->Use();

		auto wt = Matrix4DRow::Identity;
		mDebugVertexColorShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
		mDebugVertexColorShaderProgram->setMatrix4Row("uWorldTransform", wt);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mLinesVertices.size() / 6));
		glBindVertexArray(0);
	}

	void DebugRenderer::Impl::DrawDebug()
	{
		glEnable(GL_DEPTH_TEST);
		glLineWidth(3);
		if (mDrawDebug) {
			if (mDrawLines && !mLinesVertices.empty())
			{
				DrawDebugLines();
			}
			if (mDrawBoxes)
			{
				DrawDebugBoxes();
				
				DrawDebugBoxesWithMatrices();
			}
		}
		if (mDrawPersistantDebug)
		{
			if (mPersistantBoxes.empty())
				return;

			glBindVertexArray(mDebugBoxForMatricesVao);
			glLineWidth(2);
			mDebugShaderProgram->Use();
			for (const auto& box : mPersistantBoxes)
			{
				Matrix4DRow wt = box;

				wt = Matrix4DRow::CreateScale(box.GetScale() * 2);
				wt *= Matrix4DRow::CreateFromQuaternion(box.GetRotation());
				wt *= Matrix4DRow::CreateTranslation(box.GetTranslation());
				mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
				mDebugShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
				mDebugShaderProgram->setVector3f("uColor", Vector3D(0, 0, 1));

				glDrawArrays(GL_LINES, 0, 24);
			}
		}
		glDisable(GL_DEPTH_TEST);
	}

	void DebugRenderer::Impl::DrawDebugBoxesWithMatrices()
	{
		if (mBoxesWithMatrices.empty())
			return;

		glBindVertexArray(mDebugBoxForMatricesVao);
		mDebugShaderProgram->Use();
		for (const auto& box : mBoxesWithMatrices)
		{
			Matrix4DRow wt = box;

			wt = Matrix4DRow::CreateScale(box.GetScale() * 2);
			wt *= Matrix4DRow::CreateFromQuaternion(box.GetRotation());
			wt *= Matrix4DRow::CreateTranslation(box.GetTranslation());

			mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
			mDebugShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
			mDebugShaderProgram->setVector3f("uColor", Vector3D(0, 1, 0));

			glDrawArrays(GL_LINES, 0, 24);
		}
	}

	//--------------------DEBUG RENDERER-------------------------

	DebugRenderer::DebugRenderer()
		: mImpl(std::make_unique<Impl>())
	{
	}

	DebugRenderer::~DebugRenderer()
	{
		mImpl->DeleteBuffers();
		mImpl->Unload();
	}

	void DebugRenderer::Initialize(const Window& pWindow)
	{
		mImpl->Initialize(pWindow);
	}

	void DebugRenderer::Unload()
	{
		mImpl->Unload();
	}

	void DebugRenderer::Draw(IRenderer& pRenderer)
	{
		mImpl->DrawDebug();
	}

	void DebugRenderer::AddDebugLine(const Zephyrus::Debug::DebugLine& pLine)
	{
		mImpl->mLines.push_back(pLine);
		mImpl->mLinesVertices.push_back(pLine.Start.x);
		mImpl->mLinesVertices.push_back(pLine.Start.y);
		mImpl->mLinesVertices.push_back(pLine.Start.z);

		mImpl->mLinesVertices.push_back(pLine.Color.x);
		mImpl->mLinesVertices.push_back(pLine.Color.y);
		mImpl->mLinesVertices.push_back(pLine.Color.z);

		mImpl->mLinesVertices.push_back(pLine.End.x);
		mImpl->mLinesVertices.push_back(pLine.End.y);
		mImpl->mLinesVertices.push_back(pLine.End.z);

		mImpl->mLinesVertices.push_back(pLine.Color.x);
		mImpl->mLinesVertices.push_back(pLine.Color.y);
		mImpl->mLinesVertices.push_back(pLine.Color.z);
		mImpl->mNeedRecomputeLinesBuffer = true;
	}

	void DebugRenderer::AddDebugBox(const Matrix4DRow& pWorldTransform)
	{
		mImpl->mBoxesWithMatrices.push_back(pWorldTransform);
	}

	void DebugRenderer::AddDebugBox(const Zephyrus::Debug::DebugBox& pBox)
	{
		mImpl->mBoxes.push_back(pBox);
		auto v = pBox.getBoxVertices();
		mImpl->mBoxesVertices.insert(mImpl->mBoxesVertices.end(), v.begin(), v.end());
		mImpl->mNeedRecomputeBoxesBuffer = true;
	}

	void DebugRenderer::AddPersistantDebugBox(const Matrix4DRow& pWorldTransform)
	{
		mImpl->mPersistantBoxes.push_back(pWorldTransform);
	}

	void DebugRenderer::RemovePersistantDebugBox(const Matrix4DRow& pWorldTransform)
	{
		std::erase(mImpl->mPersistantBoxes, pWorldTransform);
	}

	void DebugRenderer::DrawSelectedBox(const Matrix4DRow& pWorldTransform)
	{
		glLineWidth(2);
		glBindVertexArray(mImpl->mDebugBoxForMatricesVao);

		mImpl->mDebugShaderProgram->Use();

		Matrix4DRow wt = pWorldTransform;

		wt = Matrix4DRow::CreateScale(pWorldTransform.GetScale() * 2);
		wt *= Matrix4DRow::CreateFromQuaternion(pWorldTransform.GetRotation());
		wt *= Matrix4DRow::CreateTranslation(pWorldTransform.GetTranslation());

		mImpl->SetUniforms(wt, Vector3D(1.0f, 0.7f, 0.0f));

		glDrawArrays(GL_LINES, 0, 24);
		glLineWidth(4);
	}

	void DebugRenderer::SetViewMatrix(const Matrix4DRow& pViewMatrix)
	{
		mImpl->mView = pViewMatrix;
	}

	void DebugRenderer::SetProjMatrix(const Matrix4DRow& pProjMatrix)
	{
		mImpl->mProj = pProjMatrix;
	}

	void DebugRenderer::FlushDebugElements()
	{
		mImpl->FlushDebugElements();
	}

	void DebugRenderer::FlushDebugLines()
	{
		mImpl->FlushDebugLines();
	}

	void DebugRenderer::FlushDebugBoxes()
	{
		mImpl->FlushDebugBoxes();
	}

	void DebugRenderer::SetDrawDebug(bool pDraw)
	{
		mImpl->mDrawDebug = pDraw;
	}

	void DebugRenderer::SetDrawLines(bool pDraw)
	{
		mImpl->mDrawLines = pDraw;
	}

	void DebugRenderer::SetDrawBoxes(bool pDraw)
	{
		mImpl->mDrawBoxes = pDraw;
	}

	void DebugRenderer::SetDrawSelected(bool pDraw)
	{
		mImpl->mDrawSelected = pDraw;
	}
}