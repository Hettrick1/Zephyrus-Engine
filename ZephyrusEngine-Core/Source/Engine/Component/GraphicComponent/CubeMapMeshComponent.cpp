#include "CubeMapMeshComponent.h"
#include "VertexArray.h"
#include "Actor.h"
#include "SDL.h"
#include "glew.h"

namespace Zephyrus::ActorComponent
{
	CubeMapMeshComponent::CubeMapMeshComponent(Actor* pOwner, Assets::IMesh* pMesh, Assets::ICubeMapTexture* pCubeMap, Render::IShaderProgram* pProgram)
		:MeshComponent(pOwner), mCubeMapTexture(pCubeMap)
	{
		MeshComponent::SetMesh(pMesh);
		MeshComponent::SetShaderProgram(pProgram);
	}

	CubeMapMeshComponent::~CubeMapMeshComponent()
	{
	}

	void CubeMapMeshComponent::Draw(const Zephyrus::Render::IRenderer& pRenderer)
	{
		//if (mMesh)
		//{
		//	Matrix4DRow wt = mOwner->GetTransformComponent().GetWorldTransform();
		//	mShaderProgram->Use();
		//	mShaderProgram->setMatrix4Row("uViewProj", viewProj);
		//	mShaderProgram->setMatrix4Row("uWorldTransform", wt);
		//	mShaderProgram->setVector2f("uTiling", mTiling);
		//	mShaderProgram->setFloat("uLod", mOwner->GetLod());
		//	mShaderProgram->setFloat("uTime", SDL_GetTicks());
		//	mCubeMapTexture->Bind();
		//	mMesh->Bind();
		//	if ((mShaderProgram->GetType() & ShaderProgramType::TESSELLATION_CONTROL) != 0)
		//	{
		//		//glPatchParameteri(GL_PATCH_VERTICES, 3);
		//		glDrawArrays(GL_PATCHES, 0, mMesh->GetVertexCount());
		//	}
		//	else
		//	{
		//		glDrawArrays(GL_TRIANGLES, 0, mMesh->GetVertexCount());
		//	}
		//}
	}
}
