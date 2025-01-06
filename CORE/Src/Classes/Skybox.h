#pragma once 


//other libraries
#include <glad/gl.h>
#include <stb_image.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

//standard library
#include <vector>
#include <string>
#include <iostream>

//classes
#include "Shader.h"
#include "Camera.h"
#include "Window.h"

class Skybox
{
public:

	Skybox();
	~Skybox();


	void Init(const std::vector<std::string>& faces,Window& window);
	void Render(Camera* camera);



	unsigned int GetId() { return m_cubemapTexture; }
private:

	unsigned int LoadCubemap(std::vector<std::string> faces);

private:

	unsigned int m_skyboxVAO;
	unsigned int m_skyboxVBO;
	unsigned int m_cubemapTexture;

	Shader m_shader;
};
