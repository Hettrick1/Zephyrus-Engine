#pragma once

#include "IRenderer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "DebugLine.h"
#include "Texture.h"
#include <vector>
#include "DebugRenderer.h"
#include "SkySphereComponent.h"
#include "Interface/IRenderTarget.h"

namespace Zephyrus::ActorComponent {
	class Actor;
}

using Zephyrus::ActorComponent::ActorState;
using Zephyrus::ActorComponent::Actor;
using Zephyrus::ActorComponent::SpriteComponent;
using Zephyrus::ActorComponent::MeshComponent;
using Zephyrus::ActorComponent::SkySphereComponent;
using Zephyrus::ActorComponent::CameraComponent;
using Zephyrus::Assets::VertexArrayOpenGL;
using Zephyrus::Assets::Texture;

/**
 * @brief OpenGL renderer implementation for rendering sprites, meshes, sky spheres, HUD, and debug elements.
 * Handles the rendering pipeline using OpenGL, manages renderable components, and provides debug visualization.
 */
namespace Zephyrus::Render {
	class RendererOpenGl : public IRenderer
	{
	private:
		Window* mWindow;
		VertexArrayOpenGL* mFullscreenQuadVAO;
		VertexArrayOpenGL* mVAO;
		SDL_GLContext mContext;
		std::vector<SpriteComponent*> mSprites;
		std::vector<MeshComponent*> mMeshes;
		ShaderProgram* mSpriteShaderProgram;
		Matrix4DRow mSpriteViewProj;
		Matrix4DRow mView, mProj;
		IShader* mSpriteVertexShader{ nullptr };
		IShader* mSpriteFragmentShader{ nullptr };
		ShaderProgram mSpriteShaderProgramTemp;
		IShader* mFullscreenVertexShader{ nullptr };
		IShader* mFullscreenFragmentShader{ nullptr };
		ShaderProgram* mFullscreenShaderProgram{ nullptr };
		HudManager* mHud;
		DebugRenderer* mDebugRenderer;
		bool mWireFrameMode;
		SkySphereComponent* mSkySphereComponent;
		Actor* mSelectedActor = nullptr;

	public:
		RendererOpenGl();
		virtual ~RendererOpenGl();
		RendererOpenGl(const RendererOpenGl&) = delete;
		RendererOpenGl& operator=(const RendererOpenGl&) = delete;

		// Initializes the OpenGL renderer with the given window
		bool Initialize(Window& pWindow) override;
		void BeginDraw() override;
		void Draw() override;
		void EndDraw() override;

		void RenderActiveCamera(CameraComponent* cam) override;

		void Close() override;
		void Unload() override;

		Assets::IMesh* LoadMeshFromData(Assets::MeshData& data) override;
		Assets::IFont* LoadFont(const std::string& fontPath, unsigned int pixelHeight = 128) override;
		Assets::ITexture* LoadTexture(const std::string& texturePath) override;
		IShader* LoadShader(const std::string& shaderPath, ShaderType type) override;

		void AddSprite(SpriteComponent* pSprite) override;
		void RemoveSprite(SpriteComponent* pSprite) override;

		void AddMesh(MeshComponent* pMesh) override;
		void RemoveMesh(MeshComponent* pMesh) override;

		void AddSkySphere(SkySphereComponent* pSkySphere) override;
		void RemoveSkySphere() override;

		void SetSelectedActor(Actor* pSelectedActor) override;

		// Adds a debug line to the debug renderer
		void AddDebugLine(Zephyrus::Debug::DebugLine* pLine) override;
		void RemoveDebugLine(Zephyrus::Debug::DebugLine* pLine) override;

		// Sets the view matrix for rendering
		void SetViewMatrix(const Matrix4DRow& pViewMatrix) override;
		void SetProjMatrix(const Matrix4DRow& pProjMatrix) override;

		// Draws a sprite for the given actor with the specified parameters
		void DrawSprite(Actor& pActor, Assets::ITexture* pTexture, Rectangle2D pRect, Vector2D pOrigin, IRenderer::Flip pFlipMethod) const override;
		// Draws a debug box using min/max points and a world transform
		void DrawDebugBox(Vector3D& pMin, Vector3D& pMax, Matrix4DRow pWorldTransform) override;
		// Draws a debug line between two points with hit information
		void DrawDebugLine(const Vector3D& pStart, const Vector3D& pEnd, const HitResult& pHit) override;

		void DrawSkySphere();
		void DrawMeshes();
		void DrawSprites();
		void DrawHud();
		// Draws a HUD image with the specified texture, rectangle, origin, and tint
		void DrawHudImage(Assets::ITexture* pTexture, Rectangle2D pRect, Vector2D pOrigin, Vector4D pTint) override;

		// Sets the shader program used for rendering sprites
		void SetSpriteShaderProgram(ShaderProgram& shaderProgram) override;

		// Sets the HUD manager
		void SetHud(HudManager* pHud) override;
		inline HudManager* GetHud() const override { return mHud; }

		RendererType GetType() const override { return IRenderer::RendererType::OPENGL; }
		DebugRenderer* GetDebugRenderer() const override { return mDebugRenderer; }

		// Enables or disables wireframe rendering mode
		void SetWireFrameMode(bool pWireframe) override;
		inline bool GetWireFrame() const override { return mWireFrameMode; }
	};
}