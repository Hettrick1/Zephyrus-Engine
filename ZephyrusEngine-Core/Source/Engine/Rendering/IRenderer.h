#pragma once

#include "Actor.h"
#include "AtmosphereComponent.h"
#include "Rectangle.h"
#include "Window.h"
#include "Matrix4DRow.h"
#include "Vector4D.h"
#include "../Interface/IShader.h"
#include "../Interface/IShaderProgram.h"
#include "../Material/MaterialInstance.h"

namespace Zephyrus::ActorComponent
{
	class SpriteComponent;
	class MeshComponent;
	class SkySphereComponent;
	class CameraComponent;
	class AtmosphereComponent;
}

namespace Zephyrus::Assets
{
	struct MeshData;
	class IMesh;
	class IFont;
	class ITexture2D;
	class ICubeMapTexture;
}

namespace Zephyrus::UI {
	class HudManager;
}

namespace Zephyrus::Physics
{
	struct HitResult;
}

namespace Zephyrus::Debug {
	struct DebugLine;
}

namespace Zephyrus::Material
{
	class IMaterial;
}

using Zephyrus::ActorComponent::Actor;
using Zephyrus::ActorComponent::SpriteComponent;
using Zephyrus::ActorComponent::MeshComponent;
using Zephyrus::ActorComponent::SkySphereComponent;
using Zephyrus::ActorComponent::CameraComponent;
using Zephyrus::ActorComponent::AtmosphereComponent;
using Zephyrus::Application::Window;
using Zephyrus::UI::HudManager;

namespace Zephyrus::Render {

	class DebugRenderer;

	class IRenderer
	{
	public:
		enum class Flip
		{
			None = SDL_FLIP_NONE,
			Horizontal = SDL_FLIP_HORIZONTAL,
			Vertical = SDL_FLIP_VERTICAL
		};
		enum class RendererType
		{
			SDL,
			OPENGL
		};
		virtual ~IRenderer() = default;

		virtual bool Initialize(Window& pWindow) = 0;
		virtual void BeginDraw() = 0;
		virtual void Draw() = 0;
		virtual void EndDraw() = 0;

		virtual Assets::IMesh* LoadMeshFromData(Assets::MeshData& data) = 0;
		virtual Assets::IFont* LoadFont(const std::string& fontPath, unsigned int pixelHeight = 128) = 0;
		virtual Assets::ITexture2D* LoadTexture(const std::string& texturePath) = 0;
		virtual Assets::ICubeMapTexture* LoadCubemap(const std::vector<std::string>& pCubePaths) = 0;
		virtual IShader* LoadShader(const std::string& shaderPath, ShaderType type) = 0;
		virtual IShaderProgram* LoadShaderProgram(std::vector<IShader*> pShaders) = 0;
		virtual Material::IMaterial* CreateMaterial() = 0;

		virtual void RenderActiveCamera(CameraComponent* cam) {}

		virtual void Close() = 0;
		virtual void Unload() = 0;

		virtual void AddSprite(SpriteComponent* pSprite) = 0;
		virtual void RemoveSprite(SpriteComponent* pSprite) = 0;

		virtual void AddMesh(MeshComponent* pMesh) = 0;
		virtual void RemoveMesh(MeshComponent* pMesh) = 0;

		virtual void AddSkySphere(SkySphereComponent* pSkySphere) = 0;
		virtual void RemoveSkySphere(SkySphereComponent* pSkySphere) = 0;

		virtual void AddDebugLine(Zephyrus::Debug::DebugLine* pLine) {} // DEBUG PURPOSE ONLY
		virtual void RemoveDebugLine(Zephyrus::Debug::DebugLine* pLine) {} // DEBUG PURPOSE ONLY

		virtual void SetSpriteShaderProgram(IShaderProgram* pShaderProgram) {}
		virtual void SetViewMatrix(const Matrix4DRow& pViewMatrix) {}
		virtual void SetProjMatrix(const Matrix4DRow& pProjMatrix) {}

		virtual void SetSelectedActor(Actor* pSelectedActor) {};

		virtual RendererType GetType() const = 0;

		virtual void DrawSprite(Material::MaterialInstance& pMaterial, const Matrix4DRow& pWorldTransform) const = 0;
		virtual void DrawMesh(Material::MaterialInstance& pMaterial, Assets::IMesh* pMesh, const Matrix4DRow& pWorldTransform) const = 0;
		virtual void DrawSkyBox(Material::MaterialInstance& pMaterial, Assets::IMesh* pMesh, const Matrix4DRow& pWorldTransform) const = 0;
		virtual void DrawSkySphere(Material::MaterialInstance& pMaterial, Assets::IMesh* pMesh, const Matrix4DRow& pWorldTransform) const = 0;

		virtual void DrawSprite(Actor& pActor, Assets::ITexture2D* pTex, Rectangle2D pSourceRect, Vector2D pOrigin, Flip pFlip = Flip::None) const = 0;
		virtual void DrawHudImage(Assets::ITexture2D* pTexture, Rectangle2D pRect, Vector2D pOrigin, Vector4D pTint) = 0;
		virtual void DrawDebugBox(Vector3D& pMin, Vector3D& pMax, Matrix4DRow pWorldTransform) {}
		virtual void DrawDebugLine(const Vector3D& pStart, const Vector3D& pEnd, const HitResult& pHit) {}
		virtual SDL_Renderer* ToSdlRenderer() { return nullptr; }

		virtual void SetHud(HudManager* pHud) {};
		virtual inline HudManager* GetHud() const { return nullptr; };

		virtual DebugRenderer* GetDebugRenderer() const { return nullptr; }

		virtual void SetWireFrameMode(bool pWireframe) {}
		virtual inline bool GetWireFrame() const { return false; }

		virtual void SetCameraPosition(const Vector3D& pPosition) = 0;
		virtual void AddAtmosphereComponent(AtmosphereComponent* pAtmosphereComponent) = 0;
		virtual void RemoveAtmosphereComponent(AtmosphereComponent* pAtmosphereComponent) = 0;
		
	};
}