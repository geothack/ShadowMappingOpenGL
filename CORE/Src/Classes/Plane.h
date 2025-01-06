#pragma once
#include <glad/gl.h>
#include <glm.hpp>



#include "Shader.h"

class Plane
{
public:
	Plane() = default;
	~Plane();

	void Init();
	void Render(Shader& shader);



private:
	unsigned int m_planeVBO, m_planeVAO;
};