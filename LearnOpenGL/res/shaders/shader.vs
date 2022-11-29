#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 outLightPos;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // 顶点位置属性乘以模型矩阵（不是观察和投影矩阵）变换到世界空间坐标
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    // 矩阵求逆是一项对于着色器开销很大的运算 
    // 法线矩阵被定义为「模型矩阵左上角3x3部分的逆矩阵的转置矩阵」
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    outLightPos = vec3(view * vec4(lightPos, 1.0));
}