#pragma once

#include "IRenderer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "DebugLine.h"
#include <vector>
#include "DebugRenderer.h"
#include "SkySphereComponent.h"

class Actor;
class Texture;

/**
 * @brief OpenGL renderer implementation for rendering sprites, meshes, sky spheres, HUD, and debug elements.
 * Handles the rendering pipeline using OpenGL, manages renderable components, and provides debug visualization.
 */
class RendererOpenGl : public IRenderer
{
private:
	Window* mWindow;
	VertexArray* mVAO;
	SDL_GLContext mContext;
	std::vector<SpriteComponent*> mSprites;
	std::vector<MeshComponent*> mMeshes;
	ShaderProgram* mSpriteShaderProgram;
	Matrix4DRow mSpriteViewProj;
	Matrix4DRow mView, mProj;
	Shader mSpriteVertexShader;
	Shader mSpriteFragmentShader;
	ShaderProgram mSpriteShaderProgramTemp;
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
	void Close() override;
	void Unload() override;

	void AddSprite(SpriteComponent* pSprite) override;
	void RemoveSprite(SpriteComponent* pSprite) override;

	void AddMesh(MeshComponent* pMesh) override;
	void RemoveMesh(MeshComponent* pMesh) override;

	void AddSkySphere(SkySphereComponent* pSkySphere) override;
	void RemoveSkySphere() override;

	void SetSelectedActor(Actor* pSelectedActor) override;

	// Adds a collider to the debug renderer for visualization
	void AddDebugCollider(ColliderComponent* pCol) override;
	void RemoveDebugCollider(ColliderComponent* pCol) override;
	// Adds a debug line to the debug renderer
	void AddDebugLine(DebugLine* pLine) override;
	void RemoveDebugLine(DebugLine* pLine) override;

	// Sets the view matrix for rendering
	void SetViewMatrix(const Matrix4DRow& pViewMatrix) override;
	void SetProjMatrix(const Matrix4DRow& pProjMatrix) override;

	// Draws a sprite for the given actor with the specified parameters
	void DrawSprite(Actor& pActor, Texture& pTexture, Rectangle pRect, Vector2D pOrigin, IRenderer::Flip pFlipMethod) const override;
	// Draws a debug box using min/max points and a world transform
	void DrawDebugBox(Vector3D& pMin, Vector3D& pMax, Matrix4DRow pWorldTransform) override;
	// Draws a debug line between two points with hit information
	void DrawDebugLine(const Vector3D& pStart, const Vector3D& pEnd, const HitResult& pHit) override;

	void DrawSkySphere();
	void DrawMeshes();
	void DrawSprites();
	void DrawHud();
	// Draws a HUD image with the specified texture, rectangle, origin, and tint
	void DrawHudImage(Texture& pTexture, Rectangle pRect, Vector2D pOrigin, Vector4D pTint);

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