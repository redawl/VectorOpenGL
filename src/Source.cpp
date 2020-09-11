#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Field.h"
#include <time.h>

const int numVertices = 100;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);//changes the layout of pixels with the window size
void processInput(GLFWwindow* window);//checks if the user has hit the escape key, and terminates the window if so
void initialize(unsigned int& vao, Shader& ourShader, float*& vertices);//initializes all necessary objects
void display(unsigned int& vao, Shader& ourShader, int currTime, float* vertices);//call this each loop

int main(void)
{
	srand(time(0));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(2160, 2160, "Vector Field Generator", NULL, NULL);
	
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(1);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 2160, 2160);
	glEnable(GL_PROGRAM_POINT_SIZE);

	unsigned int vao;
	Shader ourShader("src/shaders/shader.vs", "src/shaders/shader.fs", "src/shaders/shader.gs");
	Field vecField(numVertices);
	float* vertices = 0;
	vecField.Generate(vertices);
	initialize(vao, ourShader, vertices);

	int currTime = 0;
	unsigned int timeLoc = glGetUniformLocation(ourShader.ID, "time");
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0, 0.0, 0.0, -0.01f);
		glClear(GL_COLOR_BUFFER_BIT);

		if(currTime < 100)
			currTime++;
		
		glUniform1i(timeLoc, currTime);
		processInput(window);
		vecField.Generate(vertices);
		
		display(vao, ourShader, currTime, vertices);
		//ImGui::ShowDemoWindow();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	delete[] vertices;
	return 0;
}

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

void display(unsigned int& vao, Shader& ourShader, int currTime, float* vertices) {
	glBindVertexArray(vao);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * (numVertices * numVertices) * sizeof(float), vertices);
	ourShader.setInt("currTime", currTime);
	ourShader.use();
	glDrawArrays(GL_POINTS, 0, 2 * (numVertices * numVertices));
}