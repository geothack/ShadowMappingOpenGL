#pragma once 

//other libraries
#include <glad/gl.h>

//standard library 
#include <iostream>

//created classes
#include "Shader.h"
#include "Window.h"
class Depthbuffer
{
public:
	Depthbuffer();
	~Depthbuffer();

	void Use(Window& window);
	void UnBind(Window& window);
	void Init(Shader& shader);
	void Render(Shader& shader);

	unsigned int GetDepthTex() { return m_textureDepthbuffer; }

private:
	unsigned int m_quadVAO, m_quadVBO;
	unsigned int m_framebuffer;
	unsigned int m_textureDepthbuffer;
	unsigned int m_rbo;

	float near_plane = 1.0f, far_plane = 7.5f;

};