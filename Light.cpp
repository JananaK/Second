#include "Light.h"
#include "OBJObject.h"
#include "Window.h"

#define DIRECTIONAL 1
#define POINT		2
#define SPOTLIGHT	3

using namespace std;

Light::Light(int type)
{
	mode = type;
	light_dir = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Light::draw()
{
	glUniform1i(glGetUniformLocation(Window::shaderProgram, "Light.light_mode"), mode);
	glUniform3f(glGetUniformLocation(Window::shaderProgram, "Light.light_color"), light_color.x, light_color.y, light_color.z);
	glUniform3f(glGetUniformLocation(Window::shaderProgram, "Light.light_dir"), light_dir.x, light_dir.y, light_dir.z);
}
