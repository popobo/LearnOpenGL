﻿#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    // ambient 环境光照
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    // 法向量
    vec3 norm = normalize(Normal);
    // 光照方向
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    // 32是高光的反光度(Shininess)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // 环境光分量和漫反射分量相加乘以物体颜色
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}