//#pragma once

#ifndef _OBJOBJECT_H_
#define _OBJOBJECT_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <stdio.h>
#include <iostream>

#include "Light.h"

using namespace std;

class OBJObject
{
// Private variables copied from Project 1
private:
	vector<glm::vec3> ctrlPts;
	vector<glm::vec3> segmentPts;
	vector<glm::vec3> anchorPts;
	vector<glm::vec3> handlePts;

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	glm::mat4 originalWorld;

public:
	OBJObject();
	~OBJObject();

	glm::mat4 toWorld;

	//vector<glm::vec3> colors;

	void calculatePoints();	// Taken from P1
	void OBJObject::multMatrices(glm::mat4 basisM, glm::mat4x3 ctrlPoints);
	void draw(GLuint);
	void parse(const char* filepath);
	void render();

	// These variables are needed for the shader program
	GLuint VBO, VBO2, VBO3, VBO4;
	GLuint VAO, VAO2, VAO3, VAO4;
	GLuint uProjection, uModelview, uColor, uModel, uView, uChoice;
};

#endif