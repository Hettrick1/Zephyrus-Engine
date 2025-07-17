#include "CubeMapMeshComponent.h"
#include "VertexArray.h"

CubeMapMeshComponent::CubeMapMeshComponent(Actor* pOwner, Mesh* pMesh, CubeTextureMap pCubeMap, ShaderProgram* pProgram)
	:MeshComponent(pOwner, pMesh, pProgram), mCubeMapTexture(pCubeMap)
{
}

CubeMapMeshComponent::~CubeMapMeshComponent()
{
}

void CubeMapMeshComponent::Draw(Matrix4DRow viewProj)
{
	if (mMesh)
	{
		Matrix4DRow wt = mOwner->GetTransformComponent().GetWorldTransform();
		mShaderProgram.Use();
		mShaderProgram.setMatrix4Row("uViewProj", viewProj);
		mShaderProgram.setMatrix4Row("uWorldTransform", wt);
		mShaderProgram.setVector2f("uTiling", mTiling);
		mShaderProgram.setFloat("uLod", mOwner->GetLod());
		mShaderProgram.setFloat("uTime", SDL_GetTicks());
		mCubeMapTexture.SetActive();
		mMesh->GetVao()->SetActive();
		if ((mShaderProgram.GetType() & ShaderProgramType::TESSELLATION_CONTROL) != 0)
		{
			//glPatchParameteri(GL_PATCH_VERTICES, 3);
			glDrawArrays(GL_PATCHES, 0, mMesh->GetVao()->GetVerticeCount());
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, mMesh->GetVao()->GetVerticeCount());
		}
	}
}
