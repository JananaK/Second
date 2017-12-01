#include <cstring>
#include "OBJObject.h"
#include "Window.h"
#include "Light.h"

using namespace std;

#define	POINT_NUM	150

float Window::xMax = 0;
float Window::xMin = 0;
float Window::yMax = 0;
float Window::yMin = 0;
float Window::zMax = 0;
float Window::zMin = 0;

OBJObject::OBJObject()
{
	angle = 0.0f;
	calculatePoints();

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * segmentPts.size(), segmentPts.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // We've sent the vertex data over to OpenGL, but there's still something missing.
					 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}


OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

// Copied parser from HW1
void OBJObject::calculatePoints()
{
	// BEZIER CUBIC BASIS
	float cubicCoeff[16] = {
		-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0
	};

	glm::mat4 basisMatrix;
	memcpy( glm::value_ptr(basisMatrix), cubicCoeff, sizeof(cubicCoeff) );

	// BEZIER CURVE 1
	/*glm::vec3 point01(5.0f, 5.0f, 0.0f);
	glm::vec3 point02(7.0f, 7.0f, 0.0f);
	glm::vec3 point03(9.0f, 9.0f, 0.0f);
	glm::vec3 point04(11.0f, 7.0f, 0.0f);

	glm::mat4x3 G1 = glm::mat4x3(point01, point02, point03, point04);
	multMatrices(basisMatrix, G1);*/

	glm::mat4x3 curve1 = {
		5.0f, 0.0f, 0.0f,
		4.0f, 2.0f, 0.0f,
		3.0f, 3.0f, 0.0f,
		2.0f, 3.0f, 0.0f
	};

	glm::mat4x3 curve2 = {
		2.0f, 3.0f, 0.0f,
		3.0f, 3.0f, -1.0f,
		2.0f, 6.0f, -2.0f,
		1.0f, 5.0f, -2.0f
	};
	
	glm::mat4x3 curve3 = {
		1.0f, 5.0f, -2.0f,
		-1.0f, 7.0f, -2.0f,
		-2.0f, 4.0f, -1.0f,
		4.0f, 4.0f, -1.0f
	};

	glm::mat4x3 curve4 = {
		4.0f, 4.0f, -1.0f,
		7.0f, 2.0f, -2.0f,
		8.0f, 4.0f, -1.0f,
		9.0f, 4.0f, 3.0f
	};

	glm::mat4x3 curve5 = {
		9.0f, 4.0f, 3.0f,
		10.0f, 7.0f, 2.0f,
		6.0f, 8.0f, -1.0f,
		3.0f, 9.0f, -5.0f
	};

	glm::mat4x3 curve6 = {
		3.0f, 9.0f, -5.0f,
		0.0f, 7.0f, -5.0f,
		0.0f, 4.0f, -5.0f,
		0.0f, 2.0f, -5.0f
	};

	glm::mat4x3 curve7 = {
		0.0f, 2.0f, -5.0f,
		0.0f, 0.0f, -5.0f,
		0.0f, -4.0f, -5.0f,
		20.0f, -6.0f, -5.0f
	};

	glm::mat4x3 curve8 = {
		20.0f, -6.0f, 0.0f,
		22.0f, -6.0f, 5.0f,
		20.0f, -6.0f, 10.0f,
		5.0f, 0.0f, 0.0f
	};

	multMatrices(basisMatrix, curve1);
	multMatrices(basisMatrix, curve2);
	multMatrices(basisMatrix, curve3);
	multMatrices(basisMatrix, curve4);
	multMatrices(basisMatrix, curve5);
	multMatrices(basisMatrix, curve6);
	multMatrices(basisMatrix, curve7);
	multMatrices(basisMatrix, curve8);
}

void OBJObject::multMatrices(glm::mat4 basisM, glm::mat4x3 ctrlPoints)
{
	glm::vec3 segmentPoint;

	for (float i = 0.0f; i < POINT_NUM + 1 ; i++)
	{
		glm::vec4 polyB( (float)i*i*i / POINT_NUM / POINT_NUM / POINT_NUM, (float)i*i / POINT_NUM / POINT_NUM, (float)i / POINT_NUM, 1.0f);
		segmentPoint = ctrlPoints * basisM * polyB;
		segmentPts.push_back(glm::vec3(segmentPoint.x, segmentPoint.y, segmentPoint.z));
	}
}

void OBJObject::draw(GLuint shaderProgram)
{
	// We need to calculate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::Projection[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::View[0][0]);

	glBindVertexArray(VAO);

	// Tell OpenGL to draw the 151 line segments
	glDrawArrays(GL_LINE_STRIP, 0, segmentPts.size());

	glBindVertexArray(0);
}