#pragma once
#include "Component.h"
#include "MeshComponent.h"
#include "CubeTextureMap.h"
#include "Interface/IMesh.h"
#include "Interface/IShaderProgram.h"
#include "Maths.h"

using Zephyrus::Assets::CubeTextureMap;

namespace Zephyrus::ActorComponent
{
	class CubeMapMeshComponent : public MeshComponent
	{
	public:
		CubeMapMeshComponent(Actor* pOwner, Assets::IMesh* pMesh, CubeTextureMap pCubeMap, Render::IShaderProgram* pProgram = nullptr);
		virtual ~CubeMapMeshComponent();
		void Draw(const Matrix4DRow& viewProj) override;

		inline Render::IShaderProgram* GetShaderProgram() { return mShaderProgram; }

	protected:
		CubeTextureMap mCubeMapTexture;
	};
}