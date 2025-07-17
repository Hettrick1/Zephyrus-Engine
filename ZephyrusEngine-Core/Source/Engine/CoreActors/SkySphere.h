#pragma once

#include "Actor.h"
#include "Shaders/Shader.h"
#include "Texture.h"
#include "Shaders/ShaderProgram.h"
#include <vector>
#include "SkySphereComponent.h"

/**
 * @brief Represents a sky sphere or skybox actor in the scene.
 * This class manages the sky rendering using a SkySphereComponent and handles
 * the loading of textures and shaders for the sky.
 */
class SkySphere : public Actor
{
public:
	SkySphere(bool isSphere = false, std::vector<std::string> textures = {}, ShaderProgram* program = nullptr);
	~SkySphere();
	
	void Start() override;
	void Update() override;
	void Destroy() override;
private:
	ShaderProgram* mShaderProgram;
	SkySphereComponent* mSkySphereComponent;
	std::vector<std::string> mTextureToLoad;
	bool mIsSphere;
};