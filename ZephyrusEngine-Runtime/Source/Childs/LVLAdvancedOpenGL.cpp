#include "LVLAdvancedOpenGL.h"
#include "CoreActors/BasicCube.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderProgram.h"
#include "Game/Doom/DoomPlayer.h"
#include "Game/TestOpenGl/Cube.h"
#include "MeshComponent.h"
#include "Game/TestOpenGl/Sphere.h"
#include "CoreActors/SkySphere.h"
#include "Game/Planets/Planet.h"
#include "SceneManager.h"

LVLAdvancedOpenGl::LVLAdvancedOpenGl()
	: Scene()
{
}

LVLAdvancedOpenGl::~LVLAdvancedOpenGl()
{
}

void LVLAdvancedOpenGl::Start(IRenderer* renderer)
{
	Scene::Start(renderer);
	SkySphere* skySphere = new SkySphere(false, {
		"../Imports/Sprites/SkyBox/Stars/bottom.png",
		"../Imports/Sprites/SkyBox/Stars/top.png",
		"../Imports/Sprites/SkyBox/Stars/back.png",
		"../Imports/Sprites/SkyBox/Stars/left.png",
		"../Imports/Sprites/SkyBox/Stars/front.png",
		"../Imports/Sprites/SkyBox/Stars/right.png",
		});
	skySphere->Start();
	AddActor(skySphere);
	SceneManager::ActiveScene->GetRenderer()->GetDebugRenderer()->SetDrawBoxes(false);

	CubeTextureMap* cubemap = new CubeTextureMap();
	cubemap->CreateCubeTextureMap({
		"../Imports/Sprites/CubeMap/nx1.png",
		"../Imports/Sprites/CubeMap/px1.png",
		"../Imports/Sprites/CubeMap/py1.png",
		"../Imports/Sprites/CubeMap/ny1.png",
		"../Imports/Sprites/CubeMap/nz1.png",
		"../Imports/Sprites/CubeMap/pz1.png",
		});

	Shader vert, frag, tcs, tes, geom = Shader();
	vert.Load("VertFrag/CubePlanet.vert", ShaderType::VERTEX);
	frag.Load("VertFrag/CubePlanet.frag", ShaderType::FRAGMENT);
	tcs.Load("Tesselation/CubePlanet.tesc", ShaderType::TESSELLATION_CONTROL);
	tes.Load("Tesselation/CubePlanet.tese", ShaderType::TESSELLATION_EVALUATION);

	ShaderProgram* shaderProg = new ShaderProgram();
	shaderProg->Compose({ &vert, &tcs, &tes, &frag });

	vert.Load("VertFrag/CubeToSphere.vert", ShaderType::VERTEX);
	frag.Load("VertFrag/CubeToSphere.frag", ShaderType::FRAGMENT);
	tcs.Load("Tesselation/CubeToSphere.tesc", ShaderType::TESSELLATION_CONTROL);
	tes.Load("Tesselation/CubeToSphere.tese", ShaderType::TESSELLATION_EVALUATION);

	ShaderProgram* shaderProg2 = new ShaderProgram();
	shaderProg2->Compose({ &vert, &tcs, &tes, &frag });

	vert.Load("VertFrag/FirePlanet.vert", ShaderType::VERTEX);
	frag.Load("VertFrag/FirePlanet.frag", ShaderType::FRAGMENT);
	tcs.Load("Tesselation/FirePlanet.tesc", ShaderType::TESSELLATION_CONTROL);
	tes.Load("Tesselation/FirePlanet.tese", ShaderType::TESSELLATION_EVALUATION);

	ShaderProgram* shaderProg3 = new ShaderProgram(); 
	shaderProg3->Compose({ &vert, &tcs, &tes, &frag }); 

	vert.Load("VertFrag/EarthPlanet.vert", ShaderType::VERTEX);
	frag.Load("VertFrag/EarthPlanet.frag", ShaderType::FRAGMENT);
	tcs.Load("Tesselation/EarthPlanet.tesc", ShaderType::TESSELLATION_CONTROL);
	tes.Load("Tesselation/EarthPlanet.tese", ShaderType::TESSELLATION_EVALUATION);

	ShaderProgram* shaderProg4 = new ShaderProgram();
	shaderProg4->Compose({ &vert, &tcs, &tes, &frag });

	vert.Load("VertFrag/EarthPlanet.vert", ShaderType::VERTEX); 
	frag.Load("VertFrag/EarthPlanetTree.frag", ShaderType::FRAGMENT); 
	tcs.Load("Tesselation/EarthPlanet.tesc", ShaderType::TESSELLATION_CONTROL); 
	tes.Load("Tesselation/EarthPlanet.tese", ShaderType::TESSELLATION_EVALUATION);
	geom.Load("Geometry/EarthPlanet.geom", ShaderType::GEOMETRY);

	ShaderProgram* shaderProg5 = new ShaderProgram();
	shaderProg5->Compose({ &vert, &tcs, &tes, &geom, &frag });

	Planet* planet = new Planet(Vector3D(16, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg);
	AddActor(planet);
	planet->Start();

	BasicCube* cube2 = new BasicCube(Vector3D(8, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg2);
	AddActor(cube2);
	cube2->Start();

	Planet* planet2 = new Planet(Vector3D(0, 10, -1), 3);
	AddActor(planet2);
	planet2->Start();

	Planet* planet3 = new Planet(Vector3D(-8, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg3);
	AddActor(planet3);
	planet3->Start();

	Planet* planet4 = new Planet(Vector3D(-28, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg5, cubemap);
	AddActor(planet4);
	planet4->Start();

	Planet* planet5 = new Planet(Vector3D(-18, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg4, cubemap);
	AddActor(planet5);
	planet5->Start();

	Planet* planet6 = new Planet(Vector3D(-38, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg4, cubemap);
	AddActor(planet6);
	planet6->Start();

	Planet* planet7 = new Planet(Vector3D(-38, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg5, cubemap);
	AddActor(planet7);
	planet7->Start();

	DoomPlayer* player = new DoomPlayer();
	AddActor(player);
	player->Start();
}

void LVLAdvancedOpenGl::Update()
{
	Scene::Update();
}

void LVLAdvancedOpenGl::Render()
{
	Scene::Render();
}

void LVLAdvancedOpenGl::Close()
{
	Scene::Close();
}
