#pragma once
#include "Component.h"
#include "MeshComponent.h"
#include "Interface/ICubeMapTexture.h"
#include "Interface/IMesh.h"
#include "Interface/IShaderProgram.h"
#include "Maths.h"

namespace Zephyrus::ActorComponent
{
	class CubeMapMeshComponent : public MeshComponent
	{
	public:
		CubeMapMeshComponent(Actor* pOwner, Assets::IMesh* pMesh, Assets::ICubeMapTexture* pCubeMap, Render::IShaderProgram* pProgram = nullptr);
		virtual ~CubeMapMeshComponent();
		void Draw(const Zephyrus::Render::IRenderer& pRenderer) override;

		inline Render::IShaderProgram* GetShaderProgram() { return mShaderProgram; }

	protected:
		Assets::ICubeMapTexture* mCubeMapTexture;
	};
}