#include "LVLAdvancedOpenGl.h"
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
	vert = *Assets::LoadShader("VertFrag/CubePlanet.vert", ShaderType::VERTEX, "CubePlanetVert");
	frag = *Assets::LoadShader("VertFrag/CubePlanet.frag", ShaderType::FRAGMENT, "CubePlanetFrag");
	tcs = *Assets::LoadShader("Tesselation/CubePlanet.tesc", ShaderType::TESSELLATION_CONTROL, "CubePlanetTesc");
	tes = *Assets::LoadShader("Tesselation/CubePlanet.tese", ShaderType::TESSELLATION_EVALUATION, "CubePlanetTese");

	ShaderProgram* shaderProg = new ShaderProgram();
	shaderProg->Compose({ &vert, &tcs, &tes, &frag });

	vert = *Assets::LoadShader("VertFrag/CubeToSphere.vert", ShaderType::VERTEX, "CubeToSphereVert");
	frag = *Assets::LoadShader("VertFrag/CubeToSphere.frag", ShaderType::FRAGMENT, "CubeToSphereFrag");
	tcs = *Assets::LoadShader("Tesselation/CubeToSphere.tesc", ShaderType::TESSELLATION_CONTROL, "CubeToSphereTesc");
	tes = *Assets::LoadShader("Tesselation/CubeToSphere.tese", ShaderType::TESSELLATION_EVALUATION, "CubeToSphereTese");

	ShaderProgram* shaderProg2 = new ShaderProgram();
	shaderProg2->Compose({ &vert, &tcs, &tes, &frag });

	vert = *Assets::LoadShader("VertFrag/FirePlanet.vert", ShaderType::VERTEX, "FirePlanetVert");
	frag = *Assets::LoadShader("VertFrag/FirePlanet.frag", ShaderType::FRAGMENT, "FirePlanetFrag");
	tcs = *Assets::LoadShader("Tesselation/FirePlanet.tesc", ShaderType::TESSELLATION_CONTROL, "FirePlanetTesc");
	tes = *Assets::LoadShader("Tesselation/FirePlanet.tese", ShaderType::TESSELLATION_EVALUATION, "FirePlanetTese");

	ShaderProgram* shaderProg3 = new ShaderProgram(); 
	shaderProg3->Compose({ &vert, &tcs, &tes, &frag }); 

	vert = *Assets::LoadShader("VertFrag/EarthPlanet.vert", ShaderType::VERTEX, "EarthPlanetVert");
	frag = *Assets::LoadShader("VertFrag/EarthPlanet.frag", ShaderType::FRAGMENT, "EarthPlanetFrag");
	tcs = *Assets::LoadShader("Tesselation/EarthPlanet.tesc", ShaderType::TESSELLATION_CONTROL, "EarthPlanetTesc");
	tes = *Assets::LoadShader("Tesselation/EarthPlanet.tese", ShaderType::TESSELLATION_EVALUATION, "EarthPlanetTese");

	ShaderProgram* shaderProg4 = new ShaderProgram();
	shaderProg4->Compose({ &vert, &tcs, &tes, &frag });

	vert = *Assets::LoadShader("VertFrag/EarthPlanet.vert", ShaderType::VERTEX, "EarthPlanetVert");
	frag = *Assets::LoadShader("VertFrag/EarthPlanetTree.frag", ShaderType::FRAGMENT, "EarthPlanetTreeFrag");
	tcs = *Assets::LoadShader("Tesselation/EarthPlanet.tesc", ShaderType::TESSELLATION_CONTROL, "EarthPlanetTesc");
	tes = *Assets::LoadShader("Tesselation/EarthPlanet.tese", ShaderType::TESSELLATION_EVALUATION, "EarthPlanetTese");
	geom = *Assets::LoadShader("Geometry/EarthPlanet.geom", ShaderType::GEOMETRY, "EarthPlanetGeom");

	ShaderProgram* shaderProg5 = new ShaderProgram();
	shaderProg5->Compose({ &vert, &tcs, &tes, &geom, &frag });

	Planet* planet = new Planet(Vector3D(16, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg);

	BasicCube* cube2 = new BasicCube(Vector3D(8, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg2);

	Planet* planet2 = new Planet(Vector3D(0, 10, -1), 3);

	Planet* planet3 = new Planet(Vector3D(-8, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg3);

	Planet* planet4 = new Planet(Vector3D(-28, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg5, cubemap);

	Planet* planet5 = new Planet(Vector3D(-18, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg4, cubemap);

	Planet* planet6 = new Planet(Vector3D(-38, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg4, cubemap);

	Planet* planet7 = new Planet(Vector3D(-38, 10, -1), 3, Quaternion(0, 0, 0, 1), shaderProg5, cubemap);

	DoomPlayer* player = new DoomPlayer();
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
