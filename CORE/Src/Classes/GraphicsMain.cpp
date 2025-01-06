#include "GraphicsMain.h"


GraphicsMain::GraphicsMain()
{
    Init();
    Run();
}

GraphicsMain::~GraphicsMain()
{
    delete m_window;
    delete m_camera;
}


void GraphicsMain::Init()
{
    m_window = new Window("SharedLibraryCMake", 800, 600);
    m_window->CallBufferSize(); //Get the correct width and height after the screen maximizes

    m_camera = new Camera(glm::vec3(0, 5, 25));

    m_hex.Load("hex.jpg");

    
    m_depthTest = Shader("DepthTest.shader",*m_window);
    m_depthBufferShader = Shader("depthBuffer.shader",*m_window);
    m_shadows = Shader("Shadow.shader",*m_window);


    m_shadows.Use();
    m_shadows.SetInt("diffuseTexture", 0);
    m_shadows.SetInt("shadowMap", 1);
    m_shadows.SetInt("skybox", 2);

    m_skyboxFaces =
    {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };

    m_skybox.Init(m_skyboxFaces,*m_window);

    m_depthBuffer.Init(m_depthBufferShader);

    m_plane.Init();

    m_monkey.LoadModel("monkey3.obj");
}

void GraphicsMain::Run()
{
    glEnable(GL_DEPTH_TEST);
    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

    while (m_window->GetRun())
    {
        m_currentCounter = SDL_GetPerformanceCounter();
        m_deltaTime = static_cast<double>(m_currentCounter - m_previousCounter) / SDL_GetPerformanceFrequency();
        m_previousCounter = m_currentCounter;

        m_window->Event(*m_camera,m_deltaTime);

        glm::mat4 view = m_camera->GetViewMatrix();

        m_window->Clear();

        glm::mat4 lightMatrix = CalculateLightMatrix(lightPos);

        m_depthTest.Use();
        m_depthTest.SetMat4("lightSpaceMatrix", lightMatrix);

        m_depthBuffer.Use(*m_window);


        m_plane.Render(m_depthTest);

        DrawGameObject(m_monkey, CalculateTransform({ -5,0,-5 }), view, m_depthTest, &m_hex);

        DrawGameObject(m_monkey, CalculateTransform({ 5,0,-5 }), view, m_depthTest, &m_hex);

        DrawGameObject(m_monkey, CalculateTransform({ 0,2,-5 }, { 0,1,0 }), view, m_depthTest, &m_hex);



        m_depthBuffer.UnBind(*m_window);


        m_shadows.Use();
        // set light uniforms
        m_shadows.SetVec3("viewPos", m_camera->GetPos());
        m_shadows.SetVec3("lightPos", lightPos);
        m_shadows.SetMat4("lightSpaceMatrix", lightMatrix);



        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_depthBuffer.GetDepthTex());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox.GetId());

        m_plane.Render(m_shadows);

        DrawGameObject(m_monkey, CalculateTransform({ -5,2,-5 }), view, m_shadows, &m_hex);

        DrawGameObject(m_monkey, CalculateTransform({ 5,2,-5 }), view, m_shadows, &m_hex);

        DrawGameObject(m_monkey, CalculateTransform({ 0,2,-5 }, { 0,1,0 }), view, m_shadows, &m_hex);


        m_depthBuffer.Render(m_depthBufferShader);


        m_skybox.Render(m_camera);

        m_counter += 0.05f;

       
        m_window->CallBufferSize();

        m_window->SwapBuffers();


    }
}

void GraphicsMain::DrawGameObject(Model& model, glm::mat4 transform, glm::mat4 view, Shader& shader, Texture* texture)
{
    if (texture)
    {
        glActiveTexture(GL_TEXTURE0);
        texture->Use();
    }


    shader.Use();
    shader.SetMat4("model", transform);
    shader.SetMat4("view", view);

    model.Draw();
}

void GraphicsMain::DrawInstancedGameObjects(GameObject* gameObjects, Model& model, Shader& shader, Texture* texture)
{
    glm::mat4 transform(1.0);
    glm::mat4 view(1.0);
    view = m_camera->GetViewMatrix();

    if (texture)
        texture->Use();
    shader.Use();

    for (unsigned int i = 0; i < 20; ++i)
    {
        gameObjects[i].TransformPositions(glm::vec3(gameObjects[i].GetTM().GetPos()), glm::vec3(gameObjects[i].GetTM().rot.x + m_deltaTime * 0.25, gameObjects[i].GetTM().rot.y + m_deltaTime * 0.25, gameObjects[i].GetTM().rot.z + m_deltaTime * 0.25), glm::vec3(0.7, 0.7, 0.7));
        gameObjects[i].Draw(&model);
        gameObjects[i].Update(&model);

        transform = glm::translate(transform, gameObjects[i].GetTM().pos);
        transform = glm::rotate(transform, glm::radians(-45.0f), gameObjects[i].GetTM().rot);
        transform = glm::scale(transform, gameObjects[i].GetTM().scale);

        shader.SetMat4("model", transform);
        shader.SetMat4("view", view);
    }
}

glm::mat4 GraphicsMain::CalculateTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, pos);
    if (rot != glm::vec3(0.0, 0.0, 0.0))
        transform = glm::rotate(transform, m_counter * glm::radians(50.0f), rot);
    transform = glm::scale(transform, scale);
    return transform;
}

glm::mat4 GraphicsMain::CalculateLightMatrix(glm::vec3 lightPos)
{
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    return lightSpaceMatrix;
}