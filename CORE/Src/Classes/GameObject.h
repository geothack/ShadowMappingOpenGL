#pragma once 

//created classes
#include "Model.h"
#include "Shader.h"


struct Transform
{
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec3 GetPos() { return pos; }
};

class GameObject
{

public:
	GameObject();
	void TransformPositions(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	void Update(Model* model);
	void Draw(Model* model);
	void SetActive(bool set);

	bool GetActive() { return m_active; }
	//glm::mat4 getModel() { return tObject.GetModel(); }
	Transform GetTM() { return m_tObject; }


private:
	Transform m_tObject;
	Shader m_shaderIn;

	bool m_active;
	float m_counter = 0.0f;

	float m_deltaTime;
};
