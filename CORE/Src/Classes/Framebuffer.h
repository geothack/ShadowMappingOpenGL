#pragma once 

//other libraries
#include <glad/gl.h>

//standard library 
#include <iostream>

//created classes
#include "Shader.h"
class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Use();
	void Init(Shader& shader);
	void Render(Shader& shader);

	unsigned int GetColourTex() { return m_textureColorbuffer; }

private:
	unsigned int m_quadVAO, m_quadVBO;
	unsigned int m_framebuffer;
	unsigned int m_textureColorbuffer;
	unsigned int m_rbo;


};