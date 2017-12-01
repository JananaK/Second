/// TAKEN FROM LINKS ON THE HW PAGE: https://learnopengl.com/code_viewer.php?code=lighting/lighting_maps&type=vertex
#version 330 core
layout (location = 0) in vec3 trackPos;
layout (location = 1) in vec3 controlPos;
layout (location = 2) in vec3 anchorPos;
layout (location = 3) in vec3 handlePos;

out vec3 FragPos;
out vec3 FragPos2;
out vec4 color;

uniform int choice;
uniform vec4 colors;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(trackPos, 1.0f);

	if (choice == 1 ) gl_Position = projection * view * model * vec4(controlPos, 1.0f);
	if (choice == 2 ) gl_Position = projection * view * model * vec4(anchorPos, 1.0f);
	if (choice == 3 ) gl_Position = projection * view * model * vec4(handlePos, 1.0f);
    //FragPos = vec3(model * vec4(position, 1.0f));
	//FragPos = vec3(model * vec4(position, 1.0f));
	color = colors;
}
