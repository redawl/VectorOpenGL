#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "shader.h"
#include "field.h"
#include "gui.h"

#define VERSION "0.1.3"

const int NUM_PIXELS   = 40000;

// changes the layout of pixels with the window size
void framebuffer_size_callback(GLFWwindow * window, int width, int height);

// checks if the user has hit the escape key, and terminates the window if so
void processInput(GLFWwindow * window);

// initializes all necessary objects
void initialize(unsigned int& vao, Shader * shader, float * vertices);

// call this each loop
void display(unsigned int& vao, Shader * shader, float* vertices, int renderedPixels);

// scrollwheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); 

// Current zoom level
double zoomLevel = 10;

int main(int argc, char ** argv) {
    if(argc > 1 && strncmp(argv[1], "-v", 2) == 0) {
        std::cout << "Version: " << VERSION << std::endl;
        return 0;
    }

	srand(time(0));
    // Glfw init
	if(glfwInit() == GLFW_FALSE){
        std::cerr << "Failed to intilialize GLFW!" << std::endl;
        return -1;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor * primaryMonitor = glfwGetPrimaryMonitor();
	int windowHeight = glfwGetVideoMode(primaryMonitor)->height;
	int windowWidth = glfwGetVideoMode(primaryMonitor)->width;
	float pixelSize = windowHeight / 540;

    std::cout << "Initialized with window height " << windowHeight << " and width "
        << windowWidth << ", pixel size " << pixelSize << std::endl;

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Vector Field Generator", primaryMonitor, NULL);
	
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	if (!gladLoadGL(glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Gui * gui = new Gui(window, windowWidth, windowHeight);
	
	glViewport(0, 0, windowHeight, windowHeight);
	glEnable(GL_PROGRAM_POINT_SIZE);

	unsigned int vao;
	char dx[200];
	char dy[200];
	strcpy(dx, "x + y");
	strcpy(dy, "y - x");
	Shader * shader = new Shader(dx, dy);
	Field vecField(NUM_PIXELS, dx, dy);
	vecField.SetEquations(dx, dy);
	float * vertices = NULL;
	float speedFactor = 0.001f;
	
	double currX = 0;
	double currY = 0;
	double initialX = 0;
	double initialY = 0;

	initialize(vao, shader, vertices);

	float tempPixelSize = pixelSize;
	int renderedPixels = NUM_PIXELS;
	int tempPixels = NUM_PIXELS;
	float speed = 1.0f;

	while (!glfwWindowShouldClose(window)) {
		// Clear canvas for new render
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// If right mouse button is clicked, handle panning behavior
		int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (mouseState == GLFW_PRESS && initialX + currX  <= windowHeight && initialY + currY <= windowHeight) {
			glfwGetCursorPos(window, &currX, &currY);
			currX -= initialX;
			currY -= initialY;
			shader->setFloat("xOffset", 0.002f *  (float)(currX));
			shader->setFloat("yOffset", 0.002f * -(float)(currY));
		}
		else {
			glfwGetCursorPos(window, &initialX, &initialY);
			initialX -= currX;
			initialY -= currY;
		}
		
		shader->setFloat("speedFactor", speedFactor);
		shader->setFloat("pixelSize", pixelSize);

		processInput(window);
		vecField.Generate(vertices, speedFactor);
		display(vao, shader, vertices, renderedPixels);
		
		// Render imgui stuff
		gui->begin();

		if (gui->functionBox(dx, dy)) {
			vecField.SetEquations(dx, dy);

			delete shader;
			shader = new Shader(dx, dy);
		}

		if (gui->optionsBox(tempPixelSize, tempPixels, speed)) {
			pixelSize = tempPixelSize <= 100 ? tempPixelSize : 100;
            if(tempPixelSize < 1){
                pixelSize = 1;
            }
            tempPixelSize = pixelSize;
			renderedPixels = tempPixels <= 40000 ? tempPixels : 40000 ;
			speedFactor = ((speed * speed <= 100.0f) && speed != 0 ? speed * speed : 100.0f) * 0.001f;
		}
		
		gui->infoBox();

		gui->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	delete [] vertices;
	delete shader;
	delete gui;
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, height, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void initialize(unsigned int& vao, Shader * shader, float * vertices) {
	glEnable(GL_PROGRAM_POINT_SIZE);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * NUM_PIXELS * sizeof(float), vertices, GL_DYNAMIC_DRAW);

	int position = glGetAttribLocation(shader->ID, "aPos");

	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(position);

	int timePos = glGetAttribLocation(shader->ID, "time");
	glVertexAttribPointer(timePos, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(timePos);

	int fadePos = glGetAttribLocation(shader->ID, "fade");
	glVertexAttribPointer(fadePos, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(fadePos);
}

void display(unsigned int& vao, Shader * shader, float * vertices, int renderedPixels) {
	glBindVertexArray(vao);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * NUM_PIXELS * sizeof(float), vertices);
	shader->setFloat("zoomLevel", (float)zoomLevel/10);
	shader->use();
	glDrawArrays(GL_POINTS, 0, renderedPixels);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if(zoomLevel - yoffset < 50 && zoomLevel - yoffset >=10)
		zoomLevel -= yoffset;
	std::cout << "Zoom level " << zoomLevel << std::endl;
}

