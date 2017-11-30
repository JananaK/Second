#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/gtx/vector_angle.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Cube.h"
#include "OBJObject.h"
#include "shader.h"
#include "Light.h"


#define GLFW_REPEAT   2
#define GLFW_PRESS   1
#define GLFW_RELEASE   0

using namespace std;

class Window
{
public:
	static int width;
	static int height;

	static float degree;
	static float spinDegree;

	// Check for Mouse Clicking
	static bool rightMouseIsClicked;
	static bool leftMouseIsClicked;

	static float xMax, xMin, yMax, yMin, zMax, zMin;
	static glm::vec3 midPoint;
	static glm::vec2 prev_pos;
	static glm::vec2 curr_pos;

	static glm::mat4 Projection; // P for projection
	static glm::mat4 View; // V for view

	static glm::vec3 Translation;
	static glm::vec3 Scale;
	static glm::vec3 Rotate;

	static GLint shaderProgram;
	static GLint skyboxShader;

	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// Added
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 trackBallMapping(glm::vec3 point);
};

#endif
