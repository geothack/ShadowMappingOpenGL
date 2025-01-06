#include <glad/gl.h>
#include <glm.hpp>


#include <string>
#include <vector>

#include "Loader.h"

using namespace std;

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->Pos = pos;
		this->TexCoord = texCoord;
		this->Normal = Normal;
	}

public:
	glm::vec3* GetPos() { return &Pos; }
	glm::vec2* GetTexCoord() { return &TexCoord; }
	glm::vec3* GetNormal() { return &Normal; }

public:
	glm::vec3 Pos;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
};

struct Sphere
{
public:

	Sphere() {}

	Sphere(glm::vec3& pos, float radius)
	{
		this->m_pos = pos;
	}

	glm::vec3 GetPos() { return m_pos; }
	float GetRadius() { return m_radius; }

	void SetPos(glm::vec3 pos)
	{
		this->m_pos = pos;
	}

	void SetRadius(float radius)
	{
		this->m_radius = radius;
	}

private:
	glm::vec3 m_pos;
	float m_radius;
};

class Model
{
public:
	Model();
	~Model();


	void Draw();
	void Init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void LoadModel(const std::string& filename);
	void InitModel(const ModelIndex& model);
	void LoadGameVertexs(Vertex* vertices, unsigned int numVertices);
	void DrawGameVertexs();

	void UpdateSphereData(glm::vec3 pos, float radius);
	glm::vec3 GetSpherePos() { return m_meshSphere.GetPos(); }
	float GetSphereRadius() { return m_meshSphere.GetRadius(); }



private:



	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		NUM_BUFFERS
	};

	Sphere m_meshSphere;
	unsigned int m_vertexArrayObject;
	unsigned int m_vertexArrayBuffers[NUM_BUFFERS]; // create our array of buffers
	unsigned int m_EBO;
	unsigned int m_modelVertexArray;

	unsigned int m_drawCount; //how much of the vertexArrayObject do we want to draw
};



