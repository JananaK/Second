#include "window.h"

const char* window_title = "GLFW Starter Project";
Cube * skybox;
OBJObject * object;
GLint Window::skyboxShader;
GLint Window::bezierShader;

// On some systems you need to change this to the absolute path
#define SKYBOX_VERTEX_SHADER "/Users/janic/Documents/CSE167StarterCode3-master/skybox_shader.vert"
#define SKYBOX_FRAGMENT_SHADER "/Users/janic/Documents/CSE167StarterCode3-master/skybox_shader.frag"

#define VERTEX_SHADER "/Users/janic/Documents/CSE167StarterCode3-master/shader.vert"
#define FRAGMENT_SHADER "/Users/janic/Documents/CSE167StarterCode3-master/shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

glm::vec3 Window::Translation(0, 0, 0);		// keeps track of location
glm::vec3 Window::Scale(1.0f, 1.0f, 1.0f);	// keeps track of scaling and orientation
glm::vec3 Window::Rotate(1.0f, 1.0f, 1.0f);
glm::vec4 Window::black(0.0f, 0.0f, 0.0f, 0.0f);

float Window::degree = 0;					// keeps track of orbit degree
float Window::spinDegree = 0;				// keeps track of spin degree

int Window::width;
int Window::height;

bool Window::rightMouseIsClicked = false;
bool Window::leftMouseIsClicked = false;

glm::vec2 Window::prev_pos(0, 0);
glm::vec2 Window::curr_pos(0, 0);

glm::mat4 Window::Projection;
glm::mat4 Window::View;

void Window::initialize_objects()
{
	skybox = new Cube();
	object = new OBJObject();

	// Load the shader program. Make sure you have the correct filepath up top
	skyboxShader = LoadShaders(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER);
	bezierShader = LoadShaders(VERTEX_SHADER, FRAGMENT_SHADER);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);
	delete(object);
	glDeleteProgram(skyboxShader);
	glDeleteProgram(bezierShader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	Window::width = width;
	Window::height = height;

	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		Projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		View = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	// object->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the matrix mode to GL_MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	// Load the identity matrix
	glLoadIdentity();

	// Use the shader of programID
	glUseProgram(skyboxShader);
	skybox->draw(skyboxShader);

	glUseProgram(bezierShader);

	// Render the cube
	object->draw(bezierShader);
	// Render objects
	//object->render();	// Render the object

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	/*if (action == GLFW_PRESS)
	{
		// Check if 'X' was pressed, moves right
		if (key == GLFW_KEY_X && mods & GLFW_MOD_SHIFT)
		{
			object->translate(glm::vec3(1.0f, 0, 0));
		}

		// Check if 'x' was pressed, moves left
		else if (key == GLFW_KEY_X)
		{
			object->translate(glm::vec3(-1.0f, 0, 0));
		}

		// Check if 'Y' was pressed, moves up
		if (key == GLFW_KEY_Y && mods & GLFW_MOD_SHIFT)
		{
			object->translate(glm::vec3(0, 1.0f, 0));
		}

		// Check if 'y' was pressed, moves down
		else if (key == GLFW_KEY_Y)
		{
			object->translate(glm::vec3(0, -1.0f, 0));
		}

		// Check if 'Z' was pressed, moves into screen
		if (key == GLFW_KEY_Z && mods & GLFW_MOD_SHIFT)
		{
			object->translate(glm::vec3(0, 0, 1.0f));
		}

		// Check if 'z' was pressed, moves out of screen
		else if (key == GLFW_KEY_Z)
		{
			object->translate(glm::vec3(0, 0, -1.0f));
		}

		// Check if 'S' was pressed, scales up around object's center
		if (key == GLFW_KEY_S && mods & GLFW_MOD_SHIFT)
		{
			object->scale(glm::vec3(1.1f, 1.1f, 1.1f));
		}

		// Check if 's' was pressed, scales down around object's center
		else if (key == GLFW_KEY_S)
		{
			object->scale(glm::vec3(0.9f, 0.9f, 0.9f));
		}

		// Check if 'j' was pressed, center!
		if (key == GLFW_KEY_J)
		{
			object->centerScale();
		}
	}*/
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// When button is pressed
	if (action == GLFW_REPEAT || action == GLFW_PRESS) 
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			leftMouseIsClicked = true;

			// Get the cursor position
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			prev_pos = glm::vec2(xpos, ypos);
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			rightMouseIsClicked = true;

			// Get the cursor position
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			prev_pos = glm::vec2(xpos, ypos);
		}
	}

	// When button is released
	if (action == GLFW_RELEASE)
	{
		leftMouseIsClicked = false;
		rightMouseIsClicked = false;
	}

}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (leftMouseIsClicked)
	{
		// Transform camera 3vec into 4x4 matrix:
		glm::vec4 camPosition = glm::vec4(cam_pos, 1.0f);
		//glm::vec4 camDirection = glm::vec4(cam_up, 1.0f);

		float rotAngle;

		// Get 3D point for prev_pos
		glm::vec3 direction;
		glm::vec3 Point;
		glm::vec3 currVector;
		glm::vec3 prevVector = trackBallMapping(glm::vec3(prev_pos.x, prev_pos.y, 0));

		// Get 3D point for curr_pos
		glfwGetCursorPos(window, &xpos, &ypos);
		curr_pos = glm::vec2(xpos, ypos);

		currVector = trackBallMapping(glm::vec3(curr_pos.x, curr_pos.y, 0));

		direction = currVector - prevVector;
		float velocity = glm::length(direction);

		if (velocity > 0.0001) 
		{
			glm::vec3 rotAxis;
			rotAxis = glm::cross(prevVector, currVector);
			rotAxis = glm::normalize(rotAxis);

			rotAngle = velocity * 1.05;

			// Calculate angle between vectors:
			/*rotAngle = (glm::dot(prevVector, currVector));

			if (rotAngle > 1.0f) rotAngle = 1.0f;
			if (rotAngle < 0.0f) rotAngle = 0.0f;

			rotAngle /= (glm::length(prevVector) * glm::length(currVector));
			rotAngle = glm::acos(rotAngle);*/

			/*if (camPosition.z < 0)
			{
				//velocity = -1 * velocity;
				//rotAxis = glm::cross(currVector, prevVector);
				//rotAxis = glm::normalize(rotAxis);

			}*/

			//cerr << "rotAngle: " << rotAngle << endl;

			camPosition = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis) * camPosition;
			//camDirection = glm::rotate(glm::mat4(1.0f), velocity, rotAxis) * camDirection;

			cam_pos = glm::vec3(camPosition.x, camPosition.y, camPosition.z);
			//cam_up = glm::vec3(camDirection.x, camDirection.y, camDirection.z);
			View = glm::lookAt(cam_pos, cam_look_at, cam_up);

			prev_pos = curr_pos;
		}
	}
}

glm::vec3 Window::trackBallMapping(glm::vec3 point)
{
	glm::vec3 v;
	float d;
	v.x = (2.0*point.x - width) / width;
	v.y = (height - 2.0*point.y) / height;
	// v.z = 0.0;
	d = glm::length(v);
	d = (d<1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d*d);

	// NORMALIZE
	v = glm::normalize(v);

	return v;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Move object along z plane
	// object->translate(glm::vec3(0.0f, 0.0f, xoffset - yoffset));
}