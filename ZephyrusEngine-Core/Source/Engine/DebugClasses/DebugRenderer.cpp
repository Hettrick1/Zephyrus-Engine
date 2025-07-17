#include "DebugRenderer.h"
#include "DebugLine.h"
#include "ColliderComponent.h"

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
	glLineWidth(4);
	mDebugVertex.Load("Debug.vert", ShaderType::VERTEX);
	mDebugFragment.Load("Debug.frag", ShaderType::FRAGMENT);
	mDebugShaderProgram.Compose({ &mDebugVertex, &mDebugFragment });
	mView = Matrix4DRow::CreateLookAt(Vector3D(0, 0, 5), Vector3D::unitX, Vector3D::unitZ);
	mProj = Matrix4DRow::CreatePerspectiveFOV(70.0f, pWindow.GetDimensions().x, pWindow.GetDimensions().y, 0.01f, 10000.0f);
	GLfloat vertices[] = {
		// Face avant
		0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f,

		// Face arrière
		0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

		// Liaisons entre les faces avant et arrière
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f
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
	mCollider.clear();
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
	if (mDrawDebug) {
		mDebugShaderProgram.Use();
		mDebugShaderProgram.setMatrix4Row("uViewProj", mView * mProj);
		glEnable(GL_DEPTH_TEST);
		if (mDrawBoxes)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			for (auto& collider : mCollider) // DEBUG ONLY
			{
				if (collider->GetIsActive()) {
					collider->DebugDraw(pRenderer);
				}
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_BLEND);
		}
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

void DebugRenderer::AddDebugCollider(ColliderComponent* pCol)
{
	mCollider.push_back(pCol);
}

void DebugRenderer::AddDebugLine(DebugLine* pLine)
{
	mLines.push_back(pLine);
}

void DebugRenderer::DrawDebugBox(Vector3D& pMin, Vector3D& pMax, Matrix4DRow pWorldTransform)
{
	glBindVertexArray(mDebugBoxVao);

	mDebugShaderProgram.Use();
	Matrix4DRow wt = pWorldTransform;
	mDebugShaderProgram.setMatrix4Row("uWorldTransform", wt);
	mDebugShaderProgram.setVector3f("uColor", Vector3D(0, 1, 0));

	glDrawArrays(GL_LINES, 0, 24);
}

void DebugRenderer::DrawDebugLine(const Vector3D& start, const Vector3D& end, const HitResult& hit)
{
	glBindBuffer(GL_ARRAY_BUFFER, mDebugLineVbo);
	float lineVertices[] = {
		start.x, start.y, start.z,
		end.x, end.y, end.z
	};
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVertices), lineVertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mDebugShaderProgram.Use();
	Matrix4DRow worldTransform = Matrix4DRow::Identity;
	mDebugShaderProgram.setMatrix4Row("uWorldTransform", worldTransform);

	if (hit.HitActor) {
		mDebugShaderProgram.setVector3f("uColor", Vector3D(1, 0, 0));
	}
	else {
		mDebugShaderProgram.setVector3f("uColor", Vector3D(0, 1, 0));
	}

	glBindVertexArray(mDebugLineVao);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);

	if (hit.HitActor) // draw a box where it was hit
	{
		const float size = 0.1f;

		Matrix4DRow wt = Matrix4DRow::CreateScale(size * 2);
		wt *= Matrix4DRow::CreateTranslation(hit.HitPoint - size);

		mDebugShaderProgram.Use();
		mDebugShaderProgram.setMatrix4Row("uWorldTransform", wt);

		mDebugShaderProgram.setVector3f("uColor", Vector3D(1, 1, 0));

		glBindVertexArray(mDebugBoxVao);
		glDrawArrays(GL_LINES, 0, 24);
	}
}

void DebugRenderer::SetViewMatrix(Matrix4DRow pViewMatrix)
{
	mView = pViewMatrix;
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
