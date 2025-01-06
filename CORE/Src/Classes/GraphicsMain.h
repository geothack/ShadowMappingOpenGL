#pragma once 


#include <vector>

#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "GameObject.h"
#include "DepthBuffer.h"
#include "Plane.h"
#include "Skybox.h"


class GraphicsMain
{
public:

    GraphicsMain();
    ~GraphicsMain();

private: // Loop methods 
    void Init();
    void Run();

private: // Drawing of objects 
	template<unsigned int N>
	void DrawGameObject(Model& model, glm::mat4 transform, Camera* camera, Shader& shader, Texture(&texture)[N]);

	void DrawGameObject(Model& model, glm::mat4 transform, glm::mat4 view, Shader& shader, Texture* texture = nullptr);

	void DrawInstancedGameObjects(GameObject* gameObjects, Model& model, Shader& shader, Texture* texture = nullptr);

private: //Calulations 
	glm::mat4 CalculateTransform(glm::vec3 pos, glm::vec3 rot = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));
	glm::mat4 CalculateLightMatrix(glm::vec3 lightPos);


private: // Classes
    Window* m_window;
    Camera* m_camera;

    
	Shader m_shadows;
	Shader m_depthTest;
	Shader m_depthBufferShader;


    Model m_monkey;

	Depthbuffer m_depthBuffer;

	Plane m_plane;

	Skybox m_skybox;

	Texture m_hex;


private: // Other variables 
    float m_previousCounter;
    float m_currentCounter;
    float m_deltaTime;

	float m_counter;


	std::vector<string> m_skyboxFaces;
};


// For drawing models with multiple textures!!! :)

template<unsigned int N>
inline void GraphicsMain::DrawGameObject(Model& model, glm::mat4 transform, Camera* camera, Shader& shader, Texture(&texture)[N])
{
	if (texture)
	{
		for (auto i = 0; i < N; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			texture[i].Use();
		}
	}
	glm::mat4 view = camera->GetViewMatrix();

	shader.Use();
	shader.SetMat4("model", transform);
	shader.SetMat4("view", view);

	model.Draw();
}