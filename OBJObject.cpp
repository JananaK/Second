#include <cstring>
#include "OBJObject.h"
#include "Window.h"
#include "Light.h"

using namespace std;

#define	POINT_NUM	10.0f

glm::vec4 segmentColor(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 controlColor(0.0f, 1.0f, 0.0f, 1.0f);
glm::vec4 anchorColor(1.0f, 0.0f, 0.0f, 1.0f);
int choice;

OBJObject::OBJObject()
{
	calculatePoints();

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO2);
	glGenVertexArrays(1, &VAO3);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &VBO3);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * segmentPts.size(), segmentPts.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ctrlPts.size(), ctrlPts.data(), GL_STATIC_DRAW);

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


	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ctrlPts.size(), ctrlPts.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 1.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * anchorPts.size(), anchorPts.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 1.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteVertexArrays(1, &VAO3);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);
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
	glm::vec3 point01(5.0f, 0.0f, 0.0f);
	glm::vec3 point02(4.0f, 2.0f, 0.0f);
	glm::vec3 point03(3.0f, 3.0f, 0.0f);
	glm::vec3 point04(2.0f, 3.0f, 0.0f);
	glm::mat4x3 G = glm::mat4x3(point01, point02, point03, point04);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point01);
	ctrlPts.push_back(point02);
	ctrlPts.push_back(point03);
	anchorPts.push_back(point04);

	// BEZIER CURVE 2
	glm::vec3 pointA(2.0f, 3.0f, 0.0f);
	glm::vec3 point05(3.0f, 3.0f, -1.0f);
	glm::vec3 point06(2.0f, 6.0f, -2.0f);
	glm::vec3 point07(1.0f, 5.0f, -2.0f);
	G = glm::mat4x3(pointA, point05, point06, point07);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point04);
	ctrlPts.push_back(point05);
	ctrlPts.push_back(point06);
	anchorPts.push_back(point07);
	
	// BEZIER CURVE 3
	glm::vec3 pointB(1.0f, 5.0f, -2.0f);
	glm::vec3 point08(-1.0f, 7.0f, -2.0f);
	glm::vec3 point09(-2.0f, 4.0f, -1.0f);
	glm::vec3 point10(4.0f, 4.0f, -1.0f);
	G = glm::mat4x3(pointB, point08, point09, point10);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point07);
	ctrlPts.push_back(point08);
	ctrlPts.push_back(point09);
	anchorPts.push_back(point10);

	// BEZIER CURVE 4
	glm::vec3 pointD(4.0f, 4.0f, -1.0f);
	glm::vec3 point11(7.0f, 2.0f, -2.0f);
	glm::vec3 point12(8.0f, 4.0f, -1.0f);
	glm::vec3 point13(9.0f, 4.0f, 3.0f);
	G = glm::mat4x3(pointD, point11, point12, point13);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point10);
	ctrlPts.push_back(point11);
	ctrlPts.push_back(point12);
	anchorPts.push_back(point13);

	// BEZIER CURVE 5
	glm::vec3 pointE(9.0f, 4.0f, 3.0f);
	glm::vec3 point14(10.0f, 7.0f, 2.0f);
	glm::vec3 point15(6.0f, 8.0f, -1.0f);
	glm::vec3 point16(3.0f, 9.0f, -5.0f);
	G = glm::mat4x3(pointE, point14, point15, point16);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point13);
	ctrlPts.push_back(point14);
	ctrlPts.push_back(point15);
	anchorPts.push_back(point16);

	// BEZIER CURVE 6
	glm::vec3 pointF(3.0f, 9.0f, -5.0f);
	glm::vec3 point17(0.0f, 7.0f, -5.0f);
	glm::vec3 point18(0.0f, 4.0f, -5.0f);
	glm::vec3 point19(0.0f, 2.0f, -5.0f);
	G = glm::mat4x3(pointF, point17, point18, point19);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point16);
	ctrlPts.push_back(point17);
	ctrlPts.push_back(point18);
	anchorPts.push_back(point19);

	// BEZIER CURVE 7
	glm::vec3 pointG(0.0f, 2.0f, -5.0f);
	glm::vec3 point20(0.0f, 0.0f, -5.0f);
	glm::vec3 point21(0.0f, -4.0f, -5.0f);
	glm::vec3 point22(20.0f, -6.0f, -5.0f);
	G = glm::mat4x3(pointG, point20, point21, point22);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point19);
	ctrlPts.push_back(point20);
	ctrlPts.push_back(point21);
	anchorPts.push_back(point22);

	// BEZIER CURVE 8
	glm::vec3 pointH(20.0f, -6.0f, -5.0f);
	glm::vec3 point23(22.0f, -6.0f, 5.0f);
	glm::vec3 point24(20.0f, -6.0f, 10.0f);
	glm::vec3 pointI(5.0f, 0.0f, 0.0f);
	G = glm::mat4x3(pointH, point23, point24, pointI);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point22);
	ctrlPts.push_back(point23);
	ctrlPts.push_back(point24);
	anchorPts.push_back(point01);
}

void OBJObject::multMatrices(glm::mat4 basisM, glm::mat4x3 ctrlPoints)
{
	glm::vec3 segmentPoint;

	for (float i = 0.0f; i < POINT_NUM + 1.0 ; i++)
	{
		float t = (float)i / (float)POINT_NUM;
		glm::vec4 polyB( pow(t, 3), pow(t, 2), t, 1.0f);
		segmentPoint = ctrlPoints * basisM * polyB;
		segmentPts.push_back(glm::vec3(segmentPoint.x, segmentPoint.y, segmentPoint.z));

		for (int j = 0; j < ctrlPts.size(); j++)
		{
			if (segmentPoint.x == ctrlPts[j].x && segmentPoint.y == ctrlPts[j].y && segmentPoint.z == ctrlPts[j].z) {
				cerr << "X: " << segmentPoint.x << " Y: " << segmentPoint.y << " Z: " << segmentPoint.z << endl;
			}
		}
	}
}

void OBJObject::draw(GLuint shaderProgram)
{
	// We need to calculate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uChoice = glGetUniformLocation(shaderProgram, "choice");
	uColor = glGetUniformLocation(shaderProgram, "colors");
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");

	choice = 0;

	// Now send these values to the shader program
	glUniform1i(uChoice, choice);
	glUniform4fv(uColor, 1, &segmentColor[0]);
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::Projection[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::View[0][0]);

    // Draw track
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, segmentPts.size());
	glBindVertexArray(0);

	// DRAW CONTROL POINTS
	choice = 1;
	glPointSize(8);
	glUniform1i(uChoice, choice);
	glUniform4fv(uColor, 1, &controlColor[0]);
	glBindVertexArray(VAO2);
	glDrawArrays(GL_POINTS, 0, ctrlPts.size());
	glBindVertexArray(0);

	// DRAW CONTROL POINTS
	choice = 2;
	glPointSize(8);
	glUniform1i(uChoice, choice);
	glUniform4fv(uColor, 1, &anchorColor[0]);
	glBindVertexArray(VAO3);
	glDrawArrays(GL_POINTS, 0, anchorPts.size());
	glBindVertexArray(0);
}