#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


enum CameraMovement {
	FORWARD,
    BACKWARD,
	LEFT,
	RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // camera Attributes
    glm::vec3 Position; // ���λ��
    glm::vec3 Front; // ���ָ��
    glm::vec3 Up; // �������
    glm::vec3 Right; // �������
    glm::vec3 WorldUp; // ��������ϵ����
    // euler Angles
    float Yaw; // ƫ����
    float Pitch; // ������
    // camera options
    float MovementSpeed; // �ƶ��ٶ�
    float MouseSensitivity; // ������ж�
    float Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};

