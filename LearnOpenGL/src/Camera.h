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
    glm::vec3 Position; // 相机位置
    glm::vec3 Front; // 相机指向
    glm::vec3 Up; // 相机上轴
    glm::vec3 Right; // 相机右轴
    glm::vec3 WorldUp; // 世界坐标系上轴
    // euler Angles
    float Yaw; // 偏航角
    float Pitch; // 俯仰角
    // camera options
    float MovementSpeed; // 移动速度
    float MouseSensitivity; // 鼠标敏感度
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

