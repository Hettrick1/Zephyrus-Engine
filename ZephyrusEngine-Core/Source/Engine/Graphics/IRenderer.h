#pragma once

#include "Actor.h"
#include "Rectangle.h"
#include "Window.h"
#include "Matrix4DRow.h"
#include "SDL_image.h"

class Texture;
class SpriteComponent;
class MeshComponent;
class ColliderComponent;
class HudManager;
struct HitResult;
struct DebugLine;
struct DebugBox;
class DebugRenderer;
class SkySphereComponent;

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
	virtual void Close() = 0;
	virtual void Unload() = 0;

	virtual void AddSprite(SpriteComponent* pSprite) = 0;
	virtual void RemoveSprite(SpriteComponent* pSprite) = 0;

	virtual void AddMesh(MeshComponent* pMesh) = 0;
	virtual void RemoveMesh(MeshComponent* pMesh) = 0;

	virtual void AddSkySphere(SkySphereComponent* pSkySphere) = 0;
	virtual void RemoveSkySphere() = 0;

	virtual void AddDebugCollider(ColliderComponent* pCol) {} // DEBUG PURPOSE ONLY
	virtual void AddDebugLine(DebugLine* pLine) {} // DEBUG PURPOSE ONLY

	virtual void SetSpriteShaderProgram(class ShaderProgram& shaderProgram) {}
	virtual void SetViewMatrix(Matrix4DRow pViewMatrix) {}

	virtual RendererType GetType() const = 0;
	virtual void DrawSprite(Actor& pActor, Texture& pTex, Rectangle pSourceRect, Vector2D pOrigin, Flip pFlip = Flip::None) const = 0;
	virtual void DrawDebugBox(Vector3D& pMin, Vector3D& pMax, Matrix4DRow pWorldTransform) {}
	virtual void DrawDebugLine(const Vector3D& start, const Vector3D& end, const HitResult& hit) {}
	virtual SDL_Renderer* ToSdlRenderer() { return nullptr; }

	virtual void SetHud(HudManager* pHud) {};
	virtual inline HudManager* GetHud() const { return nullptr; };

	virtual DebugRenderer* GetDebugRenderer() const { return nullptr; }

	virtual void SetWireFrameMode(bool pWireframe) { }
	virtual inline bool GetWireFrame() const { return false; }
};