#pragma once
#include <glad/gl.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


class Camera
{
public:
    //Camera() = default;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
    
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch); 
    

    glm::mat4 GetViewMatrix();

    inline glm::vec3 GetPos() { return m_position; }
    

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    void MoveForward(float amt);
    

    void MoveBack(float amt);
   

    void MoveDown(float amt);


    void MoveUp(float amt);
    

    void MoveRight(float amt);
    

    void MoveLeft(float amt);
    
    

private:
    void updateCameraVectors();
  
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;
    // euler Angles
    float m_yaw;
    float m_pitch;
    // camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;
};
