// #version 330 core
// out vec4 FragColor;
// in vec3 ourColor;
// void main()
// {
//     FragColor = vec4(ourColor, 1.0);
// }

// Practic3_1
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform float mixValue;

//uniform sampler2D ourTexture;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //FragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);
    FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(-texCoord.x, texCoord.y)), mixValue);
}