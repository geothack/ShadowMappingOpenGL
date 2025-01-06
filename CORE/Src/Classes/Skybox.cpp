#include "Skybox.h"


Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

void Skybox::Init(const std::vector<std::string>& faces,Window& window)
{

    m_shader = Shader("Skybox.shader",window);

    float skyboxVertices[] = {
        // positions          
        -100.0f,  100.0f, -100.0f,
        -100.0f, -100.0f, -100.0f,
         100.0f, -100.0f, -100.0f,
         100.0f, -100.0f, -100.0f,
         100.0f,  100.0f, -100.0f,
        -100.0f,  100.0f, -100.0f,

        -100.0f, -100.0f,  100.0f,
        -100.0f, -100.0f, -100.0f,
        -100.0f,  100.0f, -100.0f,
        -100.0f,  100.0f, -100.0f,
        -100.0f,  100.0f,  100.0f,
        -100.0f, -100.0f,  100.0f,

         100.0f, -100.0f, -100.0f,
         100.0f, -100.0f,  100.0f,
         100.0f,  100.0f,  100.0f,
         100.0f,  100.0f,  100.0f,
         100.0f,  100.0f, -100.0f,
         100.0f, -100.0f, -100.0f,

        -100.0f, -100.0f,  100.0f,
        -100.0f,  100.0f,  100.0f,
         100.0f,  100.0f,  100.0f,
         100.0f,  100.0f,  100.0f,
         100.0f, -100.0f,  100.0f,
        -100.0f, -100.0f,  100.0f,

        -100.0f,  100.0f, -100.0f,
         100.0f,  100.0f, -100.0f,
         100.0f,  100.0f,  100.0f,
         100.0f,  100.0f,  100.0f,
        -100.0f,  100.0f,  100.0f,
        -100.0f,  100.0f, -100.0f,

        -100.0f, -100.0f, -100.0f,
        -100.0f, -100.0f,  100.0f,
         100.0f, -100.0f, -100.0f,
         100.0f, -100.0f, -100.0f,
        -100.0f, -100.0f,  100.0f,
         100.0f, -100.0f,  100.0f
    };


    glGenVertexArrays(1, &m_skyboxVAO);
    glGenBuffers(1, &m_skyboxVBO);
    glBindVertexArray(m_skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    m_cubemapTexture = LoadCubemap(faces);

    m_shader.Use();
    m_shader.SetInt("skybox", 0);

    unsigned int uniformCameraBlock1 = glGetUniformBlockIndex(m_shader.GetId(), "Camera");


    glUniformBlockBinding(m_shader.GetId(), uniformCameraBlock1, 0);



    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 1 * sizeof(glm::mat4));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Skybox::Render(Camera* camera)
{
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    m_shader.Use();
    glm::mat4 view = camera->GetViewMatrix(); // remove translation from the view matrix
    m_shader.SetMat4("view", view);

    // skybox cube
    glBindVertexArray(m_skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

unsigned int Skybox::LoadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;

    //stbi_set_flip_vertically_on_load(true);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}