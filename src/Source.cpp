/*--------------------------------------------------------------------------------------------------------

Includes

--------------------------------------------------------------------------------------------------------*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Field.h"
#include "Point.h"
#include <time.h>
/*--------------------------------------------------------------------------------------------------------

Constants

--------------------------------------------------------------------------------------------------------*/
const int numVertices = 200;
const float pixelSize = 4.0f;
/*--------------------------------------------------------------------------------------------------------

Function Declarations

--------------------------------------------------------------------------------------------------------*/
//changes the layout of pixels with the window size
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
//checks if the user has hit the escape key, and terminates the window if so
void processInput(GLFWwindow* window);
//initializes all necessary objects
void initialize(unsigned int& vao, Shader& ourShader, float*& vertices);
//call this each loop
void display(unsigned int& vao, Shader& ourShader, float* vertices);
//scrollwheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); 

double y_offset = 1;//current x offset from scroll wheel

int main(void)
{
/*--------------------------------------------------------------------------------------------------------

Command Line Interface

--------------------------------------------------------------------------------------------------------*/
	int sizeOfWindow = 0;
	std::cout << "Please Enter Resolution: ";
	std::cin >> sizeOfWindow;
/*--------------------------------------------------------------------------------------------------------

GLFW Window Setup

--------------------------------------------------------------------------------------------------------*/
	srand(time(0));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(sizeOfWindow, sizeOfWindow, "Vector Field Generator", NULL, NULL);
	
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
/*--------------------------------------------------------------------------------------------------------

Initialize GLAD

--------------------------------------------------------------------------------------------------------*/

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
/*--------------------------------------------------------------------------------------------------------

OpenGL Setup

--------------------------------------------------------------------------------------------------------*/

	glViewport(0, 0, sizeOfWindow, sizeOfWindow);
	glEnable(GL_PROGRAM_POINT_SIZE);

	unsigned int vao;
	Shader ourShader("src/VectorShaders/shader.vs", "src/VectorShaders/shader.fs", "src/VectorShaders/shader.gs");
	Field vecField(numVertices);
	float* vertices = 0;
	vecField.Generate(vertices);
	initialize(vao, ourShader, vertices);
	
	double currX = 0;
	double currY = 0;
	unsigned int xLoc = glGetUniformLocation(ourShader.ID, "currX");
	unsigned int yLoc = glGetUniformLocation(ourShader.ID, "currY");
	unsigned int scalingLoc = glGetUniformLocation(ourShader.ID, "scalingFactor");
	double initialX = 0;
	double initialY = 0;

/*--------------------------------------------------------------------------------------------------------

Rendering Loop

--------------------------------------------------------------------------------------------------------*/
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (mouseState == GLFW_PRESS) {
			glfwGetCursorPos(window, &currX, &currY);
			glUniform1f(xLoc, scalingFactor * (float)(currX - initialX));
			glUniform1f(yLoc, scalingFactor * -(float)(currY - initialY));
			currX -= initialX;
			currY -= initialY;
		}
		else {
			glfwGetCursorPos(window, &initialX, &initialY);
			initialX -= currX;
			initialY -= currY;
		}
		
		glUniform1f(scalingLoc, scalingFactor);
		ourShader.setFloat("pixelSize", pixelSize);
		
		processInput(window);
		vecField.Generate(vertices);
		
		display(vao, ourShader, vertices);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
/*--------------------------------------------------------------------------------------------------------

Cleanup

--------------------------------------------------------------------------------------------------------*/

	glfwTerminate();
	delete[] vertices;
	return 0;
}
/*--------------------------------------------------------------------------------------------------------

Function Implementations

--------------------------------------------------------------------------------------------------------*/


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void initialize(unsigned int& vao, Shader& ourShader, float*& vertices) {
	glEnable(GL_PROGRAM_POINT_SIZE);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * (numVertices * numVertices) * sizeof(float), vertices, GL_DYNAMIC_DRAW);

	int position = glGetAttribLocation(ourShader.ID, "aPos");

	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(position);

	int timePos = glGetAttribLocation(ourShader.ID, "time");
	glVertexAttribPointer(timePos, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(timePos);
}

void display(unsigned int& vao, Shader& ourShader, float * vertices) {
	glBindVertexArray(vao);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * (numVertices * numVertices) * sizeof(float), vertices);
	ourShader.setFloat("Scale", (float)y_offset);
	ourShader.use();
	glDrawArrays(GL_POINTS, 0, (numVertices * numVertices));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if(y_offset - yoffset < 5 && y_offset - yoffset >=1)
		y_offset -= yoffset;
	std::cout << y_offset << std::endl;
}