#include "RendererOpenGl.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Maths.h"
#include "Actor.h"
#include "glew.h"
#include "HudManager.h"
#include "TextRenderer.h"
#include "Assets.h"
#include "CameraComponent.h"
#include "../MeshOpenGL.h"
#include "../FontOpenGL.h"
#include "../TextureOpenGL.h"
#include "Material/Material.h"
#include "CubemapTextureOpenGL.h"
#include "ShaderOpenGL.h"
#include "AtmosphereComponent.h"
#include <algorithm>

using Zephyrus::Assets::AssetsManager;
namespace Zephyrus::Render {
	RendererOpenGl::RendererOpenGl()
		: mWireFrameMode(false)
	{
	}

	RendererOpenGl::~RendererOpenGl()
	{
		if (mVAO != nullptr) {
			delete mVAO;
		}
		if (mWindow) {
			SDL_GL_DeleteContext(mContext);
			mWindow = nullptr;
		}
		delete mHud;
		delete mDebugRenderer;
	}

	bool RendererOpenGl::Initialize(Window& pWindow)
	{
		mWindow = &pWindow;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		mContext = SDL_GL_CreateContext(mWindow->GetSdlWindow());
		SDL_GL_MakeCurrent(mWindow->GetSdlWindow(), mContext);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			ZP_CORE_ERROR("Failed to initialize glew");
			return false;
		}
		glGetError();

		if (IMG_Init(IMG_INIT_PNG) == 0)
		{
			ZP_CORE_ERROR("Failed to initialize SDL_Image");
		}
		mSpriteVertexShader = AssetsManager::LoadShader("BasicHudImage.vert", ShaderType::VERTEX, "BasicHudImageVert");
		mSpriteFragmentShader = AssetsManager::LoadShader("BasicHudImage.frag", ShaderType::FRAGMENT, "BasicHudImageFrag");
		mSpriteShaderProgramTemp = AssetsManager::LoadShaderProgram({ mSpriteVertexShader, mSpriteFragmentShader }, "simpleSpriteSP");
		SetSpriteShaderProgram(mSpriteShaderProgramTemp);

		mVAO = new VertexArrayOpenGL(Zephyrus::Assets::spriteVertices, 32);
		mFullscreenQuadVAO = new VertexArrayOpenGL(Zephyrus::Assets::fullscreenQuadVertices, 32);

		mFullscreenVertexShader = AssetsManager::LoadShader("VertFrag/FullscreenQuad.vert", ShaderType::VERTEX, "FullscreenQuadvert");
		mFullscreenFragmentShader = AssetsManager::LoadShader("VertFrag/FullscreenQuad.frag", ShaderType::FRAGMENT, "FullscreenQuadfrag");
		mFullscreenShaderProgram = AssetsManager::LoadShaderProgram({ mFullscreenVertexShader, mFullscreenFragmentShader }, "FullscreenQuadSP");

		mSpriteViewProj = Matrix4DRow::CreateOrtho(static_cast<float>(pWindow.GetDimensions().x), static_cast<float>(pWindow.GetDimensions().y), 0.1f, 100000);
		mView = Matrix4DRow::CreateLookAt(Vector3D(0, 0, 5), Vector3D::unitX, Vector3D::unitZ);
		mProj = Matrix4DRow::CreatePerspectiveFOV(70.0f, mWindow->GetDimensions().x, mWindow->GetDimensions().y, 0.1f, 10000.0f);

		mHud = new HudManager();
		mDebugRenderer = new DebugRenderer();
		mDebugRenderer->Initialize(*mWindow);

		mFrameUBO.Initialize();
		
		return true;
	}

	void RendererOpenGl::BeginDraw()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		mFrameData.cameraPos = mCameraPosition;

		mFrameData.time = static_cast<float>(SDL_GetTicks()) / 1000.0f;
		mFrameData.screenWidth = mWindow->GetDimensions().x;
		mFrameData.screenHeight = mWindow->GetDimensions().y;
		
		mFrameData.fogStart = mAtmosphereComponents.empty() ? 50.0f : mAtmosphereComponents[0]->GetFogStart();
		mFrameData.fogEnd   = mAtmosphereComponents.empty() ? 200.0f : mAtmosphereComponents[0]->GetFogEnd();
		mFrameData.fogColor = mAtmosphereComponents.empty() ? Vector3D(0.6f, 0.7f, 0.9f) : mAtmosphereComponents[0]->GetFogColor();
		
		mFrameUBO.UpdateData(mFrameData);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, mFrameUBO.GetBuffer());
		mSpriteViewProj = Matrix4DRow::CreateOrtho(static_cast<float>(mWindow->GetDimensions().x), static_cast<float>(mWindow->GetDimensions().y), 0.000001f, 100000);
	}

	void RendererOpenGl::Draw()
	{
		DrawSkySphere();
		DrawMeshes();
		mDebugRenderer->Draw(*this);
		DrawSprites();
		DrawHud();
		if (mSelectedActor)
		{
			mDebugRenderer->DrawSelectedBox(mSelectedActor->GetTransformComponent().GetWorldTransform());
		}
	}

	void RendererOpenGl::EndDraw()
	{
		SDL_GL_SwapWindow(mWindow->GetSdlWindow());
	}

	void RendererOpenGl::RenderActiveCamera(CameraComponent* cam)
	{		
		if (!cam || !cam->GetRenderTarget()) return;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, static_cast<int>(mWindow->GetDimensions().x), static_cast<int>(mWindow->GetDimensions().y));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH);
		mFullscreenQuadVAO->SetActive();
		mFullscreenShaderProgram->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cam->GetRenderTarget()->GetColorTexture());

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glEnable(GL_DEPTH);
		
		EndDraw();
	}

	void RendererOpenGl::Close()
	{
		SDL_GL_DeleteContext(mContext);
		delete mVAO;
	}

	void RendererOpenGl::Unload()
	{
		mSprites.clear();
		mMeshes.clear();
		if (mHud)
		{
			mHud->Unload();
		}
		mDebugRenderer->Unload();
		mSkySphereComponents.clear();
		mAtmosphereComponents.clear();
	}

	IMesh* RendererOpenGl::LoadMeshFromData(Assets::MeshData& data)
	{
		return new MeshOpenGL(data);
	}

	Assets::IFont* RendererOpenGl::LoadFont(const std::string& fontPath, unsigned int pixelHeight)
	{
		Assets::FontOpenGL* font = new Assets::FontOpenGL();
		font->Load(fontPath, pixelHeight);
		return font;
	}

	Assets::ITexture2D* RendererOpenGl::LoadTexture(const std::string& texturePath)
	{
		Assets::TextureOpenGL* texture = new Assets::TextureOpenGL();
		texture->Load(texturePath);
		return texture;
	}

	Assets::ICubeMapTexture* RendererOpenGl::LoadCubemap(const std::vector<std::string>& pCubePaths)
	{
		Assets::CubemapTextureOpenGL* cubemap = new Assets::CubemapTextureOpenGL();
		cubemap->Load(pCubePaths);
		return cubemap;
	}

	IShader* RendererOpenGl::LoadShader(const std::string& shaderPath, ShaderType type)
	{
		ShaderOpenGL* shader = new ShaderOpenGL();
		shader->Load(shaderPath, type);
		ZP_LOAD("Shader " + shaderPath + " successfully loaded");
		return shader;
	}

	IShaderProgram* RendererOpenGl::LoadShaderProgram(std::vector<IShader*> pShaders)
	{
		ShaderProgram* program = new ShaderProgram();
		program->Compose(pShaders);
		return program;
	}

	Material::IMaterial* RendererOpenGl::CreateMaterial()
	{
		return new Material::Material();
	}

	void RendererOpenGl::AddSprite(SpriteComponent* pSprite)
	{
		int spriteDrawOrder = pSprite->GetDrawOrder();
		std::vector<SpriteComponent*>::iterator sc;
		for (sc = mSprites.begin(); sc != mSprites.end(); ++sc)
		{
			if (spriteDrawOrder < (*sc)->GetDrawOrder())
			{
				break;
			}
		}
		mSprites.insert(sc, pSprite);
	}

	void RendererOpenGl::RemoveSprite(SpriteComponent* pSprite)
	{
		std::vector<SpriteComponent*>::iterator sc;
		sc = std::find(mSprites.begin(), mSprites.end(), pSprite);
		mSprites.erase(sc);
	}

	void RendererOpenGl::AddMesh(MeshComponent* pMesh)
	{
		mMeshes.push_back(pMesh);
	}

	void RendererOpenGl::RemoveMesh(MeshComponent* pMesh)
	{
		std::vector<MeshComponent*>::iterator sc;
		sc = std::find(mMeshes.begin(), mMeshes.end(), pMesh);
		mMeshes.erase(sc);
	}

	void RendererOpenGl::AddSkySphere(SkySphereComponent* pSkySphere)
	{
		if (std::find(mSkySphereComponents.begin(), mSkySphereComponents.end(), pSkySphere) == mSkySphereComponents.end())
		{
			mSkySphereComponents.push_back(pSkySphere);
		}
	}

	void RendererOpenGl::RemoveSkySphere(SkySphereComponent* pSkySphere)
	{
		mSkySphereComponents.erase(std::remove(mSkySphereComponents.begin(), mSkySphereComponents.end(), pSkySphere), mSkySphereComponents.end());
	}

	void RendererOpenGl::SetSelectedActor(Actor* pSelectedActor)
	{
		mSelectedActor = pSelectedActor;
	}

	void RendererOpenGl::AddDebugLine(Zephyrus::Debug::DebugLine* pLine)
	{
		mDebugRenderer->AddDebugLine(pLine);
	}

	void RendererOpenGl::RemoveDebugLine(Zephyrus::Debug::DebugLine* pLine)
	{
		mDebugRenderer->RemoveDebugLine(pLine);
	}

	void RendererOpenGl::SetViewMatrix(const Matrix4DRow& pViewMatrix)
	{
		mView = pViewMatrix;
		mDebugRenderer->SetViewMatrix(pViewMatrix);
	}

	void RendererOpenGl::SetProjMatrix(const Matrix4DRow& pProjMatrix)
	{
		mProj = pProjMatrix;
		mDebugRenderer->SetProjMatrix(pProjMatrix);
	}

	void RendererOpenGl::DrawSprite(Material::MaterialInstance& pMaterial, const Matrix4DRow& pWorldTransform) const
	{
		Matrix4DRow viewProj = mView * mProj;
		pMaterial.Use(&pWorldTransform, &viewProj);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void RendererOpenGl::DrawMesh(Material::MaterialInstance& pMaterial, Assets::IMesh* pMesh, const Matrix4DRow& pWorldTransform) const
	{
		Matrix4DRow viewProj = mView * mProj;
		pMaterial.Use(&pWorldTransform, &viewProj);
		pMesh->Bind();
		if ((pMaterial.GetBaseMaterial()->GetShaderProgram()->GetType() & ShaderProgramType::TESSELLATION_CONTROL) != 0)
		{
			glDrawArrays(GL_PATCHES, 0, pMesh->GetVertexCount());
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, pMesh->GetVertexCount());
		}
	}

	void RendererOpenGl::DrawSkyBox(Material::MaterialInstance& pMaterial, Assets::IMesh* pMesh, const Matrix4DRow& pWorldTransform) const
	{
		Matrix4DRow skyView = Matrix4DRow::DeleteTranslation(mView);
		Matrix4DRow viewProj = skyView * mProj;
		pMaterial.Use(&Matrix4DRow::Identity, &viewProj);
		mSkySphereComponents[0]->GetMesh()->Bind();
		glDrawArrays(GL_PATCHES, 0, mSkySphereComponents[0]->GetMesh()->GetVertexCount());
	}

	void RendererOpenGl::DrawSkySphere(Material::MaterialInstance& pMaterial, Assets::IMesh* pMesh, const Matrix4DRow& pWorldTransform) const
	{
		Matrix4DRow skyView = Matrix4DRow::DeleteTranslation(mView);
		Matrix4DRow viewProj = skyView * mProj;
		pMaterial.Use(&Matrix4DRow::Identity, &viewProj);

		mSkySphereComponents[0]->GetMesh()->Bind();

		glDrawArrays(GL_TRIANGLES, 0, mSkySphereComponents[0]->GetMesh()->GetVertexCount());
	}

	void RendererOpenGl::DrawSprite(Actor& pActor, Assets::ITexture2D* pTexture, Rectangle2D pRect, Vector2D pOrigin, IRenderer::Flip pFlipMethod) const
	{
		if (mSpriteShaderProgram == nullptr)
		{
			return;
		}
		mSpriteShaderProgram->Use();
		pTexture->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void RendererOpenGl::DrawDebugBox(Vector3D& pMin, Vector3D& pMax, Matrix4DRow pWorldTransform)
	{
		mDebugRenderer->DrawDebugBox(pMin, pMax, pWorldTransform);
	}

	void RendererOpenGl::DrawDebugLine(const Vector3D& pStart, const Vector3D& pEnd, const HitResult& pHit)
	{
		mDebugRenderer->DrawDebugLine(pStart, pEnd, pHit);
	}

	void RendererOpenGl::DrawSkySphere()
	{
		if (!mSkySphereComponents.empty()) {
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			mSkySphereComponents[0]->Draw(*this);
			glDepthMask(GL_TRUE);
		}
	}

	void RendererOpenGl::DrawMeshes()
	{
		int drawMethod = mWireFrameMode ? GL_LINE : GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, drawMethod);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		MeshComponent* activeMesh = nullptr;
		for (MeshComponent* m : mMeshes)
		{
			if (m->GetOwner()->GetState() == ActorState::Active)
			{
				m->Draw(*this);
			}
		}
	}

	void RendererOpenGl::DrawSprites()
	{
		if (mSpriteShaderProgram == nullptr)
		{
			return;
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mSpriteShaderProgram->Use();
		mSpriteShaderProgram->setMatrix4Row("uViewProj", mView * mProj);
		mVAO->SetActive();

		std::sort(mSprites.begin(), mSprites.end(), [](const auto& a, const auto& b)
			{
				return a->GetDrawOrder() < b->GetDrawOrder();
			});

		for (Zephyrus::ActorComponent::SpriteComponent* sprite : mSprites) {
			Matrix4DRow world = sprite->GetWorldTransform();
			mSpriteShaderProgram->setMatrix4Row("uWorldTransform", world);
			mSpriteShaderProgram->setVector4f("uTint", Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
			sprite->Draw(*this);
		}
	}

	void RendererOpenGl::DrawHud()
	{
		if (mHud)
		{
			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			mHud->Draw(*this);
			glDisable(GL_CULL_FACE);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
	}

	void RendererOpenGl::DrawHudImage(Assets::ITexture2D* pTexture, Rectangle2D pRect, Vector2D pOrigin, Vector4D pTint)
	{
		if (mSpriteShaderProgram == nullptr)
		{
			return;
		}

		mSpriteShaderProgram->Use();
		mSpriteShaderProgram->setMatrix4Row("uViewProj", mSpriteViewProj);
		Vector3D adjustedPosition = Vector3D(
			pRect.position.x - (pOrigin.x * pRect.dimensions.x),
			pRect.position.y - (pOrigin.y * pRect.dimensions.y),
			0
		);
		Matrix4DRow scaleMat = Matrix4DRow::CreateScale(Vector3D(pRect.dimensions.x, pRect.dimensions.y, 0));
		Matrix4DRow world = scaleMat * Matrix4DRow::CreateTranslation(adjustedPosition);
		mSpriteShaderProgram->setMatrix4Row("uWorldTransform", world);
		mSpriteShaderProgram->setVector4f("uTint", pTint);
		mVAO->SetActive();
		pTexture->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void RendererOpenGl::SetSpriteShaderProgram(IShaderProgram* shaderProgram)
	{
		mSpriteShaderProgram = shaderProgram;
		mSpriteShaderProgram->Use();
	}

	void RendererOpenGl::SetHud(HudManager* pHud)
	{
		mHud->Unload();
		mHud = pHud;
	}

	void RendererOpenGl::SetWireFrameMode(bool pWireframe)
	{
		mWireFrameMode = pWireframe;
	}
}