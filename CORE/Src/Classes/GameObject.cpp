#include "GameObject.h"


GameObject::GameObject()
{
	m_active = true;
}

void GameObject::Draw(Model* model)
{
	model->Draw();
}

void GameObject::SetActive(bool set)
{
	m_active = set;
}

void GameObject::Update(Model* model)
{
	model->UpdateSphereData(m_tObject.GetPos(), 0.62f);
}

void GameObject::TransformPositions(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	m_tObject.pos = pos;
	m_tObject.rot = rot;
	m_tObject.scale = scale;
}