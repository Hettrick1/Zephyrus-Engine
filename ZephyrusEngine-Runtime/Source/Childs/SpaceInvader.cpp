#include "SpaceInvader.h"
#include "Game/SpaceInvader/SpaceInvaderPlayer.h"
#include "Game/SpaceInvader/GroundActor.h"

SpaceInvader::SpaceInvader()
	: mPlayer(nullptr)
{
}

SpaceInvader::~SpaceInvader()
{
	delete mPlayer;

}

void SpaceInvader::Start(IRenderer* renderer)
{
	Scene::Start(renderer);
	mPlayer = new SpaceInvaderPlayer();
	mPlayer->Start();
	AddActor(mPlayer);
	GroundActor* ground = new GroundActor(Vector3D(0, 50, 0), Vector3D(100, 1, 0), Quaternion(0,0));
	ground->Start();
	AddActor(ground);
	GroundActor* ground1 = new GroundActor(Vector3D(0, 750, 0), Vector3D(100, 1, 0), Quaternion(0,0));
	ground1->Start();
	AddActor(ground1);
	GroundActor* ground2 = new GroundActor(Vector3D(50, 0, 0), Vector3D(1, 100, 0), Quaternion(0,0));
	ground2->Start();
	AddActor(ground2);
	GroundActor* ground3 = new GroundActor(Vector3D(750, 0, 0), Vector3D(1, 100, 0), Quaternion(0,0));
	ground3->Start();
	AddActor(ground3);
;
}

void SpaceInvader::Update()
{
	Scene::Update();
}

void SpaceInvader::Render()
{
	Scene::Render();
}

void SpaceInvader::Close()
{
}
