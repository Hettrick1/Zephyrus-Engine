#include "RendererOpenGl.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "Maths.h"
#include "Actor.h"
#include "Texture.h"
#include "glew.h"
#include "HudManager.h"
#include "TextRenderer.h"

RendererOpenGl::RendererOpenGl()
	: mVAO(nullptr), mWindow(nullptr), mSpriteShaderProgram(nullptr), mHud(nullptr), mDebugRenderer(nullptr), mWireFrameMode(false), mSkySphereComponent(nullptr)
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
	delete mSpriteShaderProgram;
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
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Log::Error(LogType::Video, "Failed to initialize glew");
		return false;
	}
	glGetError();

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		Log::Error(LogType::Video, "Failed to initialize SDL_Image");
	}

	mSpriteVertexShader.Load("Simple.vert", ShaderType::VERTEX); 
	mSpriteFragmentShader.Load("Simple.frag", ShaderType::FRAGMENT);  
	mSpriteShaderProgramTemp.Compose({ &mSpriteVertexShader, &mSpriteFragmentShader });
	SetSpriteShaderProgram(mSpriteShaderProgramTemp);

	mVAO = new VertexArray(spriteVertices, 4);
	mSpriteViewProj = Matrix4DRow::CreateOrtho(static_cast<float>(pWindow.GetDimensions().x), static_cast<float>(pWindow.GetDimensions().y), 0.000001f, 100000);
	mView = Matrix4DRow::CreateLookAt(Vector3D(0, 0, 5), Vector3D::unitX, Vector3D::unitZ);
	mProj = Matrix4DRow::CreatePerspectiveFOV(70.0f, mWindow->GetDimensions().x, mWindow->GetDimensions().y, 0.01f, 10000.0f);

	mHud = new HudManager();
	mDebugRenderer = new DebugRenderer();
	mDebugRenderer->Initialize(*mWindow);

	return true;
}

void RendererOpenGl::BeginDraw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererOpenGl::Draw()
{
	DrawSkySphere();
	DrawMeshes();
	mDebugRenderer->Draw(*this);
	DrawSprites();
	DrawHud();
}

void RendererOpenGl::EndDraw()
{
	SDL_GL_SwapWindow(mWindow->GetSdlWindow());
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
	mHud->Unload();
	mDebugRenderer->Unload();
	mSkySphereComponent = nullptr;
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
	if (mSkySphereComponent != nullptr) {
		Log::Info("You had already a skysphere, the old one has been replaced");
	}
	mSkySphereComponent = pSkySphere;
}

void RendererOpenGl::RemoveSkySphere()
{
	mSkySphereComponent = nullptr;
}

void RendererOpenGl::AddDebugCollider(ColliderComponent* pCol)
{
	mDebugRenderer->AddDebugCollider(pCol);
}

void RendererOpenGl::AddDebugLine(DebugLine* pLine)
{
	mDebugRenderer->AddDebugLine(pLine);
}

void RendererOpenGl::SetViewMatrix(Matrix4DRow pViewMatrix)
{
	mView = pViewMatrix;
	mDebugRenderer->SetViewMatrix(pViewMatrix);
}

void RendererOpenGl::DrawSprite(Actor& pActor, Texture& pTexture, Rectangle pRect, Vector2D pOrigin, IRenderer::Flip pFlipMethod) const
{
	if (mSpriteShaderProgram == nullptr)
	{
		return;
	}
	mSpriteShaderProgram->Use();
	pTexture.SetActive();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RendererOpenGl::DrawDebugBox(Vector3D& pMin, Vector3D& pMax, Matrix4DRow pWorldTransform)
{
	mDebugRenderer->DrawDebugBox(pMin, pMax, pWorldTransform);
}

void RendererOpenGl::DrawDebugLine(const Vector3D& start, const Vector3D& end, const HitResult& hit)
{
	mDebugRenderer->DrawDebugLine(start, end, hit);
}

void RendererOpenGl::DrawSkySphere()
{
	if (mSkySphereComponent != nullptr) {
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		mSkySphereComponent->GetShaderProgram().Use();
		mSkySphereComponent->GetShaderProgram().setMatrix4Row("uWorld", Matrix4DRow::Identity);
		Matrix4DRow skyView = Matrix4DRow::DeleteTranslation(mView);
		mSkySphereComponent->GetShaderProgram().setMatrix4Row("uViewProj", skyView * mProj);
		mSkySphereComponent->GetVao()->SetActive();
		glBindTexture(mSkySphereComponent->GetTextureType(), mSkySphereComponent->GetTextureIndex());
		GLenum drawMode = mSkySphereComponent->GetTextureType() == GL_TEXTURE_2D ? GL_TRIANGLES : GL_PATCHES;
		glDrawArrays(drawMode, 0, mSkySphereComponent->GetVao()->GetVerticeCount());
		glDepthMask(GL_TRUE);
	}
}

void RendererOpenGl::DrawMeshes()
{
	int drawMethod = mWireFrameMode ? GL_LINE : GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, drawMethod);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	for (MeshComponent* m : mMeshes) 
	{
		m->Draw(mView * mProj);
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
	for (SpriteComponent* sprite : mSprites) {
		Matrix4DRow world = sprite->GetWorldTransform();
		mSpriteShaderProgram->setMatrix4Row("uWorldTransform", world);
		mSpriteShaderProgram->setVector4f("uTint", Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
		sprite->Draw(*this);
	}
}

void RendererOpenGl::DrawHud()
{
	glEnable(GL_CULL_FACE); 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mHud->Draw(*this); 
	glDisable(GL_CULL_FACE); 
	glDisable(GL_BLEND); 
}

void RendererOpenGl::DrawHudImage(Texture& pTexture, Rectangle pRect, Vector2D pOrigin, Vector4D pTint)
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
	pTexture.SetActive();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RendererOpenGl::SetSpriteShaderProgram(ShaderProgram& shaderProgram)
{
	mSpriteShaderProgram = &shaderProgram;
	mSpriteShaderProgram->Use();
}

void RendererOpenGl::SetHud(HudManager* pHud)
{
	mHud = pHud;
}

void RendererOpenGl::SetWireFrameMode(bool pWireframe)
{
	mWireFrameMode = pWireframe;
}
