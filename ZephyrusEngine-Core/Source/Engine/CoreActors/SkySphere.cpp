#include "SkySphere.h"
#include "SceneManager.h"

SkySphere::SkySphere(bool pIsSphere,std::vector<std::string> pTextures, ShaderProgram* pProgram)
	:Actor(), mShaderProgram(pProgram), mSkySphereComponent(nullptr), mTextureToLoad(pTextures), mIsSphere(pIsSphere)
{
	Start();
	mScene.AddActor(this);
}

SkySphere::~SkySphere()
{
}

void SkySphere::Start()
{
	Actor::Start();
	mSkySphereComponent = new SkySphereComponent(this, mIsSphere, mTextureToLoad);
}

void SkySphere::Update()
{
	Actor::Update();
}

void SkySphere::Destroy()
{
	Actor::Destroy();
}
