// Practice2_1
// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
// out vec3 ourColor;
// void main()
// {
//     gl_Position = vec4(-aPos.x, -aPos.y, -aPos.z, 1.0);
//     ourColor = aColor;
// }

// Practice2_2
// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
// out vec3 ourColor;
// uniform vec3 change;
// void main()
// {
//     gl_Position = vec4(aPos.x + change.x, aPos.y + change.y, aPos.z + change.z, 1.0);
//     ourColor = aColor;
// }

// Practice2_3
// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
// out vec3 ourColor;
// uniform vec3 change;
// void main()
// {
//     gl_Position = vec4(aPos.x + change.x, aPos.y + change.y, aPos.z + change.z, 1.0);
//     ourColor = aPos;
// }


#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    texCoord = aTexCoord;
}