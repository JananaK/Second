//#pragma once
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/gtx/vector_angle.hpp>

#include "Cube.h"
#include "OBJObject.h"
#include "shader.h"
#include "Window.h"

using namespace std;

class Light {
public:
	int mode;

	glm::vec3 light_color;
	glm::vec3 light_pos;
	glm::vec3 light_dir;

	float cons_att;
	float linear_att;
	float quad_att;

	float cutoff_angle;
	float exponent;

	Light(int type);
	void draw();

	
};
#endif