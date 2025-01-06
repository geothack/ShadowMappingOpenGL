#include "Model.h"

void Model::Init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	ModelIndex model;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.Positions.push_back(*vertices[i].GetPos());
		model.TexCoords.push_back(*vertices[i].GetTexCoord());
		model.Normals.push_back(*vertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < numIndices; i++)
		model.Indices.push_back(indices[i]);

	InitModel(model);
}

void Model::InitModel(const ModelIndex& model)
{

	m_drawCount = model.Indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject); //generate a vertex array and store it in the VAO
	glBindVertexArray(m_vertexArrayObject); //bind the VAO (any operation that works on a VAO will work on our bound VAO - binding)

	glGenBuffers(4, m_vertexArrayBuffers); //generate our buffers based of our array of data/buffers - GLuint vertexArrayBuffers[NUM_BUFFERS];

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VERTEXBUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model.Positions.size() * sizeof(model.Positions[0]), &model.Positions[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU (determined by type)
	glEnableVertexAttribArray(POSITION_VERTEXBUFFER);
	glVertexAttribPointer(POSITION_VERTEXBUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model.TexCoords.size() * sizeof(model.TexCoords[0]), &model.TexCoords[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	glEnableVertexAttribArray(TEXCOORD_VB);
	glVertexAttribPointer(TEXCOORD_VB, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.Normals[0]) * model.Normals.size(), &model.Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_VB);
	glVertexAttribPointer(NORMAL_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.Indices.size() * sizeof(model.Indices[0]), &model.Indices[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU

	glBindVertexArray(0); // unbind our VAO
}

void Model::LoadGameVertexs(Vertex* vertices, unsigned int numVertices)
{
	m_drawCount = numVertices;

	glGenVertexArrays(1, &m_modelVertexArray);
	glBindVertexArray(m_modelVertexArray);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textCoords;

	positions.reserve(numVertices);
	textCoords.reserve(numVertices);

	for (int i = 0; i < numVertices; i++)
	{
		positions.push_back(vertices[i].Pos);
		textCoords.push_back(vertices[i].TexCoord);
	}

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VERTEXBUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(textCoords[0]), &textCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Model::LoadModel(const std::string& filename)
{
	ModelIndex model = ModelArray(filename).ToModelIndex();
	InitModel(model);
	Sphere meshSphere();
}

Model::~Model()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject); // delete arrays
}

Model::Model()
{
	m_drawCount = NULL;
}
void Model::Draw()
{
	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, drawCount);

	glBindVertexArray(0);
}

void Model::DrawGameVertexs()
{
	glBindVertexArray(m_modelVertexArray);
	glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
	glBindVertexArray(0);
}

void Model::UpdateSphereData(glm::vec3 pos, float radius)
{
	m_meshSphere.SetPos(pos);
	m_meshSphere.SetRadius(radius);
}