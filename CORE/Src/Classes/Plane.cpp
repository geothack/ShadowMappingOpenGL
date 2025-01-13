#include "Plane.h"

Plane::~Plane()
{
}

void Plane::Init()
{
    float planeVertices[] = {
        // positions            // texcoords  // normals
         25.0f, -0.5f,  25.0f,  25.0f,  0.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.5f,  25.0f,   0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.5f, -25.0f,   0.0f, 25.0f, 0.0f, 1.0f, 0.0f,

         25.0f, -0.5f,  25.0f,  25.0f,  0.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.5f, -25.0f,   0.0f, 25.0f, 0.0f, 1.0f, 0.0f,
         25.0f, -0.5f, -25.0f,  25.0f, 25.0f, 0.0f, 1.0f, 0.0f,
    };
    
    
    glGenVertexArrays(1, &m_planeVAO);
    glGenBuffers(1, &m_planeVBO);
    glBindVertexArray(m_planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glBindVertexArray(0);
}

void Plane::Render(Shader& shader)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::scale(model, glm::vec3(10, 1, 10));
    shader.SetMat4("model", model);
    glBindVertexArray(m_planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
