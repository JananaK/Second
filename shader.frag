#version 330 core

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec4 color;
//in vec4 color2;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 finalColor;
//out vec4 finalColor2;

void main()
{
	finalColor = color;
	//finalColor = color2;
}