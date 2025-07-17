#include "testGlPlayer.h"
#include "Scene.h"
#include "BooleanActions.h"
#include "InputManager.h"
#include "PlayerController2D.h"
#include "Physics/CollisionManager.h"
#include "BoxCollider2DComponent.h"


testGlPlayer::testGlPlayer() :
	Actor()
{
}

testGlPlayer::~testGlPlayer()
{
}

void testGlPlayer::Start()
{
	Actor::Start();
	mTransformComponent.RotateZ(45);
}

void testGlPlayer::Update()
{
	Actor::Update();
}

void testGlPlayer::Destroy()
{
	Actor::Destroy();
}



