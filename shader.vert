/// TAKEN FROM LINKS ON THE HW PAGE: https://learnopengl.com/code_viewer.php?code=lighting/lighting_maps&type=vertex
#version 330 core
layout (location = 0) in vec3 position;

out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
}
