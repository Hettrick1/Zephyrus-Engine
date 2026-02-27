#include "pch.h"
#include "CubeMapMeshComponent.h"
#include "VertexArray.h"
#include "Actor.h"
#include "glew.h"

namespace Zephyrus::ActorComponent
{
	CubeMapMeshComponent::CubeMapMeshComponent(Actor* pOwner)
		: MeshComponent(pOwner)
	{
	}

	std::vector<PropertyDescriptor> CubeMapMeshComponent::GetProperties()
	{
		return MeshComponent::GetProperties();
	}

	void CubeMapMeshComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		MeshComponent::Deserialize(pReader);
	}

	void CubeMapMeshComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		MeshComponent::Serialize(pWriter);
	}

	CubeMapMeshComponent::~CubeMapMeshComponent()
	{
	}

	void CubeMapMeshComponent::Draw(const Zephyrus::Render::IRenderer& pRenderer)
	{
		// if (mMesh)
		// {
		// 	Matrix4DRow wt = mOwner->GetTransformComponent().GetWorldTransform();
		// 	mShaderProgram->Use();
		// 	mShaderProgram->setMatrix4Row("uViewProj", viewProj);
		// 	mShaderProgram->setMatrix4Row("uWorldTransform", wt);
		// 	mShaderProgram->setVector2f("uTiling", mTiling);
		// 	mShaderProgram->setFloat("uLod", mOwner->GetLod());
		// 	mShaderProgram->setFloat("uTime", SDL_GetTicks());
		// 	mCubeMapTexture->Bind();
		// 	mMesh->Bind();
		// 	if ((mShaderProgram->GetType() & ShaderProgramType::TESSELLATION_CONTROL) != 0)
		// 	{
		// 		//glPatchParameteri(GL_PATCH_VERTICES, 3);
		// 		glDrawArrays(GL_PATCHES, 0, mMesh->GetVertexCount());
		// 	}
		// 	else
		// 	{
		// 		glDrawArrays(GL_TRIANGLES, 0, mMesh->GetVertexCount());
		// 	}
		// }
	}
}
