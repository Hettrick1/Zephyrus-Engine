#pragma once
#include "Component.h"
#include "MeshComponent.h"
#include "CubeTextureMap.h"
#include "Interface/IMesh.h"
#include "Maths.h"

using Zephyrus::Assets::CubeTextureMap;
using Zephyrus::Render::ShaderProgram;

namespace Zephyrus::ActorComponent
{
	class CubeMapMeshComponent : public MeshComponent
	{
	public:
		CubeMapMeshComponent(Actor* pOwner, Assets::IMesh* pMesh, CubeTextureMap pCubeMap, ShaderProgram* pProgram = nullptr);
		virtual ~CubeMapMeshComponent();
		void Draw(const Matrix4DRow& viewProj) override;

		inline ShaderProgram& GetShaderProgram() { return mShaderProgram; }

	protected:
		CubeTextureMap mCubeMapTexture;
	};
}