#include <cstring>
#include "OBJObject.h"
#include "Window.h"
#include "Light.h"

using namespace std;

#define	POINT_NUM	10.0f

glm::vec4 segmentColor(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 controlColor(0.0f, 1.0f, 0.0f, 1.0f);
glm::vec4 anchorColor(1.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 handleColor(0.0f, 0.5f, 0.5f, 1.0f);
int choice;

OBJObject::OBJObject()
{
	toWorld = glm::mat4(1.0f);
	parse("sphere.obj");
	calculatePoints();

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO2);
	glGenVertexArrays(1, &VAO3);
	glGenVertexArrays(1, &VAO4);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &VBO4);

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


	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * handlePts.size(), handlePts.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 1.
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
	glDeleteVertexArrays(1, &VAO4);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &VBO4);
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

	// BEZIER POINTS 1
	glm::vec3 point24(20.0f, -6.0f, 10.0f);
	glm::vec3 point02(4.0f, 2.0f, 0.0f);
	glm::vec3 point03(3.0f, 3.0f, 0.0f);

	glm::vec3 point01((point24.x/2) + (point02.x/2), (point24.y / 2) + (point02.y / 2), (point24.z / 2) + (point02.z / 2));

	// BEZIER POINTS 2
	glm::vec3 point05(3.0f, 3.0f, -1.0f);
	glm::vec3 point06(2.0f, 6.0f, -2.0f);

	glm::vec3 point04((point03.x / 2) + (point05.x / 2), (point03.y / 2) + (point05.y / 2), (point03.z / 2) + (point05.z / 2));

	// BEZIER POINTS 3
	glm::vec3 point08(-1.0f, 7.0f, -2.0f);
	glm::vec3 point09(-2.0f, 4.0f, -1.0f);

	glm::vec3 point07((point06.x / 2) + (point08.x / 2), (point06.y / 2) + (point08.y / 2), (point06.z / 2) + (point08.z / 2));

	// BEZIER POINTS 4
	glm::vec3 point11(7.0f, 2.0f, -2.0f);
	glm::vec3 point12(8.0f, 4.0f, -1.0f);

	glm::vec3 point10((point09.x / 2) + (point11.x / 2), (point09.y / 2) + (point11.y / 2), (point09.z / 2) + (point11.z / 2));

	// BEZIER POINTS 5
	glm::vec3 point14(10.0f, 7.0f, 2.0f);
	glm::vec3 point15(6.0f, 8.0f, -1.0f);

	glm::vec3 point13((point12.x / 2) + (point14.x / 2), (point12.y / 2) + (point14.y / 2), (point12.z / 2) + (point14.z / 2));

	// BEZIER POINTS 6
	glm::vec3 point17(0.0f, 7.0f, -5.0f);
	glm::vec3 point18(0.0f, 4.0f, -5.0f);

	glm::vec3 point16((point15.x / 2) + (point17.x / 2), (point15.y / 2) + (point17.y / 2), (point15.z / 2) + (point17.z / 2));

	// BEZIER POINTS 7
	glm::vec3 point20(0.0f, 0.0f, -5.0f);
	glm::vec3 point21(0.0f, -4.0f, -5.0f);

	glm::vec3 point19((point18.x / 2) + (point20.x / 2), (point18.y / 2) + (point20.y / 2), (point18.z / 2) + (point20.z / 2));

	// BEZIER POINTS 8
	glm::vec3 point23(22.0f, -6.0f, 5.0f);

	glm::vec3 point22((point21.x / 2) + (point23.x / 2), (point21.y / 2) + (point23.y / 2), (point21.z / 2) + (point23.z / 2));

	// BEZIER CURVE 1
	glm::mat4x3 G = glm::mat4x3(point01, point02, point03, point04);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point01);
	ctrlPts.push_back(point02);
	ctrlPts.push_back(point03);
	anchorPts.push_back(point04);
	handlePts.push_back(point03);
	handlePts.push_back(point05);
	
	// BEZIER CURVE 2
	G = glm::mat4x3(point04, point05, point06, point07);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point04);
	ctrlPts.push_back(point05);
	ctrlPts.push_back(point06);
	anchorPts.push_back(point07);
	handlePts.push_back(point06);
	handlePts.push_back(point08);
	
	// BEZIER CURVE 3
	G = glm::mat4x3(point07, point08, point09, point10);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point07);
	ctrlPts.push_back(point08);
	ctrlPts.push_back(point09);
	anchorPts.push_back(point10);
	handlePts.push_back(point09);
	handlePts.push_back(point11);

	// BEZIER CURVE 4
	G = glm::mat4x3(point10, point11, point12, point13);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point10);
	ctrlPts.push_back(point11);
	ctrlPts.push_back(point12);
	anchorPts.push_back(point13);
	handlePts.push_back(point12);
	handlePts.push_back(point14);

	// BEZIER CURVE 5
	G = glm::mat4x3(point13, point14, point15, point16);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point13);
	ctrlPts.push_back(point14);
	ctrlPts.push_back(point15);
	anchorPts.push_back(point16);
	handlePts.push_back(point15);
	handlePts.push_back(point17);

	// BEZIER CURVE 6
	G = glm::mat4x3(point16, point17, point18, point19);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point16);
	ctrlPts.push_back(point17);
	ctrlPts.push_back(point18);
	anchorPts.push_back(point19);
	handlePts.push_back(point18);
	handlePts.push_back(point20);

	// BEZIER CURVE 7
	G = glm::mat4x3(point19, point20, point21, point22);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point19);
	ctrlPts.push_back(point20);
	ctrlPts.push_back(point21);
	anchorPts.push_back(point22);
	handlePts.push_back(point21);
	handlePts.push_back(point23);

	// BEZIER CURVE 8
	G = glm::mat4x3(point22, point23, point24, point01);
	multMatrices(basisMatrix, G);
	ctrlPts.push_back(point22);
	ctrlPts.push_back(point23);
	ctrlPts.push_back(point24);
	anchorPts.push_back(point01);
	handlePts.push_back(point24);
	handlePts.push_back(point02);
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

	// DRAW ANCHOR POINTS
	choice = 2;
	glPointSize(8);
	glUniform1i(uChoice, choice);
	glUniform4fv(uColor, 1, &anchorColor[0]);
	glBindVertexArray(VAO3);
	glDrawArrays(GL_POINTS, 0, anchorPts.size());
	glBindVertexArray(0);

	// DRAW HANDLE POINTS
	choice = 3;
	glPointSize(8);
	glUniform1i(uChoice, choice);
	glUniform4fv(uColor, 1, &handleColor[0]);
	glBindVertexArray(VAO4);
	glDrawArrays(GL_LINES, 0, handlePts.size());
	glBindVertexArray(0);
}

// Also "loadData" for Rasterization
void OBJObject::parse(const char *filepath)
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE *fp;		// file pointer
	glm::vec3 point;
	float x, y, z;	// vertex coordinates
	float r, g, b;	// vertex color
	int c1, c2;		// characters read from file

					// make the file name configurable so you load form other files
	fopen_s(&fp, filepath, "rb");

	// ERROR CHECK: just in case the file can't be found or is corrupt
	if (fp == NULL) { cerr << "error loading file" << endl; exit(-1); }

	while (!feof(fp)) {
		c1 = fgetc(fp);
		c2 = fgetc(fp);

		// Vertex "v" case
		if (c1 == 'v' && c2 == ' ')
		{
			fscanf_s(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			vertices.push_back(glm::vec3(x, y, z));
		}

		// Vertex normal "vn" case
		else if (c1 == 'v' && c2 == 'n')
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			point = glm::normalize(glm::vec3(x, y, z));
			point.x = (point.x + 1) / 2;
			point.y = (point.y + 1) / 2;
			point.z = (point.z + 1) / 2;
			normals.push_back(point);
		}
	}
	fclose(fp);   // close the file when done
}

void OBJObject::render()
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

	glBegin(GL_POINTS);

	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		glColor3f(1.0f, 1.0f, 1.0f /*normals[i].x, normals[i].y, normals[i].z*/);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}