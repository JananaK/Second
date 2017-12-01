// TAKEN FROM LINKS ON THE HW PAGE: https://learnopengl.com/#!Advanced-OpenGL/Cubemaps
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 textureDir;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    textureDir = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}
