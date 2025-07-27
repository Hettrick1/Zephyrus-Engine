#include "Planet.h"
#include "Assets.h"
#include "CameraComponent.h"
#include "CameraManager.h"
#include "Timer.h"
#include "Scene.h"

Planet::Planet(Vector3D pPos, Vector3D pSize, Quaternion pRotation, ShaderProgram* program, CubeTextureMap* pCubemap)
	:Actor(pPos, pSize, pRotation), mShaderProgram(program), mCubemap(pCubemap)
{
	Start();
	mScene.AddActor(this);
}

Planet::~Planet()
{
	delete mShaderProgram;
	delete mCubemap;
}

void Planet::Start()
{
	Actor::Start();
	Mesh* mesh = Assets::LoadMesh("../Imports/Meshes/cube.obj", "cube");
	Shader vert, frag, tcs, tes = Shader();
	vert = *Assets::LoadShader("VertFrag/PlanetsNoise.vert", ShaderType::VERTEX, "PlanetsNoiseVert");
	frag = *Assets::LoadShader("VertFrag/PlanetsNoise.frag", ShaderType::FRAGMENT, "PlanetsNoiseFrag");
	tcs = *Assets::LoadShader("Tesselation/PlanetsNoise.tesc", ShaderType::TESSELLATION_CONTROL, "PlanetsNoiseTesc");
	tes = *Assets::LoadShader("Tesselation/PlanetsNoise.tese", ShaderType::TESSELLATION_EVALUATION, "PlanetsNoiseTese");

	if (mShaderProgram == nullptr)
	{
		ShaderProgram* shaderProg = new ShaderProgram();
		shaderProg->Compose({ &vert, &tcs, &tes, &frag });
		mShaderProgram = shaderProg;
	}

	if (mCubemap == nullptr)
	{
		mCubemap = new CubeTextureMap();
		mCubemap->CreateCubeTextureMap({
			"../Imports/Sprites/CubeMap/nx.png",
			"../Imports/Sprites/CubeMap/px.png",
			"../Imports/Sprites/CubeMap/py.png",
			"../Imports/Sprites/CubeMap/ny.png",
			"../Imports/Sprites/CubeMap/nz.png",
			"../Imports/Sprites/CubeMap/pz.png",
		});
	}

	CubeMapMeshComponent* meshComponent = new CubeMapMeshComponent(this, mesh, *mCubemap, mShaderProgram);
}

void Planet::Update()
{
	Actor::Update();
	Vector3D camPos = CameraManager::Instance().GetCurrentCamera()->GetWorldTransform().GetTranslation();
	Vector3D dist = camPos - GetTransformComponent().GetPosition();
	float distSq = dist.LengthSq();

	if (distSq < 1800.0f)
		mLod = 32;
	else if (distSq < 2800.0f)
		mLod = 10;
	else if (distSq < 3800.0f)
		mLod = 4;
	else
		mLod = 2;

	//mTransformComponent.RotateZ(Timer::deltaTime * 50);
	//mTransformComponent.RotateX(Timer::deltaTime * 50);
}

void Planet::Destroy()
{
	Actor::Destroy();
}