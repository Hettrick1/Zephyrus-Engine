#include "pch.h"
#include "DebugRenderer.h"
#include "AssetsManager.h"
#include "EngineContentIds.h"
#include "glew.h"

#include <unordered_map>

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
		std::unordered_map<unsigned, std::vector<float>> mLinesVertices;
		std::unordered_map<unsigned, std::vector<float>> mBoxesVertices;
		std::vector<Matrix4DRow> mBoxesWithMatrices; // deprecated
		std::vector<Zephyrus::Debug::PersistantDebugBox> mPersistantBoxes;

		unsigned mNbOfLineVertices = 0;
		unsigned mNbOfBoxVertices = 0;

		unsigned mDebugBoxForMatricesVao, mDebugBoxForMatricesVbo, mDebugBoxVao, mDebugBoxVbo, mDebugLineVao, mDebugLineVbo;

		bool mDrawDebug = true, mDrawLines = true, mDrawBoxes = true, mDrawSelected = false, mDrawPersistantDebug = true;

		bool mNeedRecomputeBoxesBuffer = true;
		bool mNeedRecomputeLinesBuffer = true;

		void Initialize(const Window& pWindow);
		void DeleteBuffers();
		void Unload();

		void FlushDebugElements();
		void FlushDebugLines(int index = -1);
		void FlushDebugBoxes(int index = -1);

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
		mLinesVertices.clear();
		mBoxesVertices.clear();
		mBoxesWithMatrices.clear();
		mPersistantBoxes.clear();
	}

	void DebugRenderer::Impl::FlushDebugElements()
	{
		mBoxesVertices.clear();
		mBoxesWithMatrices.clear();
		mLinesVertices.clear();
		mNeedRecomputeBoxesBuffer = true;
		mNeedRecomputeLinesBuffer = true;
	}

	void DebugRenderer::Impl::FlushDebugLines(int index)
	{
		if (index < 0)
		{
			for (auto vector : mLinesVertices)
			{
				vector.second.clear();
			}
			mLinesVertices.clear();
			mNeedRecomputeLinesBuffer = true;
			return;
		}
		if (!mLinesVertices.contains(index))
		{
			ZP_CORE_WARN("Trying to flush non existing debug lines.");
			return;
		}
		mLinesVertices[index].clear();
		mNeedRecomputeLinesBuffer = true;
	}

	void DebugRenderer::Impl::FlushDebugBoxes(int index)
	{
		if (index < 0)
		{
			for (auto vector : mBoxesVertices)
			{
				vector.second.clear();
			}
			mBoxesVertices.clear();
			mBoxesWithMatrices.clear();
			mNeedRecomputeBoxesBuffer = true;
			return;
		}
		if (!mBoxesVertices.contains(index))
		{
			ZP_CORE_WARN("Trying to flush non existing debug boxes.");
			return;
		}
		mBoxesVertices[index].clear();
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
		if (mNeedRecomputeBoxesBuffer)
		{
			glBindVertexArray(mDebugBoxVao);
			glBindBuffer(GL_ARRAY_BUFFER, mDebugBoxVbo);

			std::vector<float> vertices;
			for (auto vector : mBoxesVertices)
			{
				vertices.insert(vertices.end(), vector.second.begin(), vector.second.end());
			}

			mNbOfBoxVertices = static_cast<unsigned>(vertices.size() / 6);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

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
		glDrawArrays(GL_LINES, 0, mNbOfBoxVertices);
		glBindVertexArray(0);
	}

	void DebugRenderer::Impl::DrawDebugLines()
	{
		if (mNeedRecomputeLinesBuffer)
		{
			glBindVertexArray(mDebugLineVao);
			glBindBuffer(GL_ARRAY_BUFFER, mDebugLineVbo);

			std::vector<float> vertices;
			for (auto vector : mLinesVertices)
			{
				vertices.insert(vertices.end(), vector.second.begin(), vector.second.end());
			}

			mNbOfLineVertices = static_cast<unsigned>(vertices.size() / 6);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

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
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mNbOfLineVertices));
		glBindVertexArray(0);
	}

	void DebugRenderer::Impl::DrawDebug()
	{
		glEnable(GL_DEPTH_TEST);
		glLineWidth(2);
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
			mDebugShaderProgram->Use();
			for (const auto& box : mPersistantBoxes)
			{
				Matrix4DRow wt = box.WorldTransform;

				glLineWidth(box.LineWidth);

				mDebugShaderProgram->setMatrix4Row("uWorldTransform", wt);
				mDebugShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
				mDebugShaderProgram->setVector3f("uColor", box.Color);

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

	void DebugRenderer::AddDebugLine(const Zephyrus::Debug::DebugLine& pLine, int index)
	{
		unsigned vectorIndex = 0;
		if (index > 0)
		{
			vectorIndex = index;
		}
		auto& vector = mImpl->mLinesVertices[vectorIndex];

		vector.push_back(pLine.Start.x);
		vector.push_back(pLine.Start.y);
		vector.push_back(pLine.Start.z);

		vector.push_back(pLine.Color.x);
		vector.push_back(pLine.Color.y);
		vector.push_back(pLine.Color.z);

		vector.push_back(pLine.End.x);
		vector.push_back(pLine.End.y);
		vector.push_back(pLine.End.z);

		vector.push_back(pLine.Color.x);
		vector.push_back(pLine.Color.y);
		vector.push_back(pLine.Color.z);

		mImpl->mNeedRecomputeLinesBuffer = true;
	}

	void DebugRenderer::AddDebugBox(const Matrix4DRow& pWorldTransform)
	{
		mImpl->mBoxesWithMatrices.push_back(pWorldTransform);
	}

	void DebugRenderer::AddDebugBox(const Zephyrus::Debug::DebugBox& pBox, int index)
	{
		unsigned vectorIndex = 0;
		if (index > 0)
		{
			vectorIndex = index;
		}
		auto& vector = mImpl->mBoxesVertices[vectorIndex];

		auto v = pBox.getBoxVertices();
		vector.insert(vector.end(), v.begin(), v.end());
		mImpl->mNeedRecomputeBoxesBuffer = true;
	}

	void DebugRenderer::AddPersistantDebugBox(const Zephyrus::Debug::PersistantDebugBox& pBox)
	{
		mImpl->mPersistantBoxes.push_back(pBox);
	}

	void DebugRenderer::RemovePersistantDebugBox(const Zephyrus::Debug::PersistantDebugBox& pBox)
	{
		std::erase(mImpl->mPersistantBoxes, pBox);
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

	void DebugRenderer::FlushDebugLines(int index)
	{
		mImpl->FlushDebugLines(index);
	}

	void DebugRenderer::FlushDebugBoxes(int index)
	{
		mImpl->FlushDebugBoxes(index);
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