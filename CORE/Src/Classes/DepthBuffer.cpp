#include "DepthBuffer.h"

Depthbuffer::Depthbuffer()
{
}

Depthbuffer::~Depthbuffer()
{
    glDeleteFramebuffers(1, &m_framebuffer);
}

void Depthbuffer::Use(Window& window)
{
    glViewport(0, 0, window.GetWidth() - 100, window.GetHeight() - 100);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
}

void Depthbuffer::UnBind(Window& window)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Depthbuffer::Init(Shader& shader)
{

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    shader.Use();
    shader.SetInt("depthMap", 0);



    glGenFramebuffers(1, &m_framebuffer);

    glGenTextures(1, &m_textureDepthbuffer);
    glBindTexture(GL_TEXTURE_2D, m_textureDepthbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2000, 2000, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureDepthbuffer, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Depthbuffer::Render(Shader& shader)
{
    
    shader.Use();
    shader.SetFloat("near_plane", near_plane);
    shader.SetFloat("far_plane", far_plane);
    //glBindVertexArray(m_quadVAO);
    glBindTexture(GL_TEXTURE_2D, m_textureDepthbuffer);	// use the color attachment texture as the texture of the quad plane
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //glBindVertexArray(0);
}