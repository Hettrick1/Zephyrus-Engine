#include "SkySphere.h"
#include "SceneManager.h"

SkySphere::SkySphere(bool isSphere,std::vector<std::string> textures, ShaderProgram* program)
	:Actor(), mShaderProgram(program), mSkySphereComponent(nullptr), mTextureToLoad(textures), mIsSphere(isSphere)
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
