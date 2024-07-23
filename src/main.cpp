#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "Shader.h"
#include "Field.h"
#include "Point.h"
#include <time.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#define VERSION "0.1.2"

const int NUM_VERTICES = 200;
const int NUM_PIXELS   = NUM_VERTICES * NUM_VERTICES;

// changes the layout of pixels with the window size
void framebuffer_size_callback(GLFWwindow * window, int width, int height);

// checks if the user has hit the escape key, and terminates the window if so
void processInput(GLFWwindow * window);

// initializes all necessary objects
void initialize(unsigned int& vao, Shader& ourShader, float*& vertices);

// call this each loop
void display(unsigned int& vao, Shader& ourShader, float* vertices, int renderedPixels);

// scrollwheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); 

double y_offset = 1;//current y offset from scroll wheel

int main(int argc, char ** argv) {
    if(argc > 1 && strncmp(argv[1], "-v", 2) == 0) {
        std::cout << "Version: " << VERSION << std::endl;
        return 0;
    }

	srand(time(0));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int windowHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
	int windowWidth = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	float pixelSize = windowHeight / 540;

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Vector Field Generator", glfwGetPrimaryMonitor(), NULL);
	
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

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int padding = windowWidth / 216;
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

    // Don't create imgui.ini
    io.IniFilename = NULL;
    io.LogFilename = NULL;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
	ImVec2 FwindowSize = ImVec2(windowWidth - windowHeight - padding - padding, ((windowHeight / 20) * 2) - padding - padding);
	ImVec2 FwindowPos = ImVec2(windowHeight + padding, padding);
	ImVec2 OwindowSize = ImVec2(FwindowSize.x, FwindowSize.y * 2);
	ImVec2 OwindowPos = ImVec2(FwindowPos.x, FwindowSize.y + padding + padding);
	ImVec2 IwindowSize = ImVec2(FwindowSize.x, windowHeight - FwindowSize.y - OwindowSize.y - padding - padding - padding - padding);
	ImVec2 IwindowPos = ImVec2(FwindowPos.x, OwindowPos.y + OwindowSize.y + padding);

	glViewport(0, 0, windowHeight, windowHeight);
	glEnable(GL_PROGRAM_POINT_SIZE);

	unsigned int vao;
	std::string changeX = "x + y";
	std::string changeY = "y - x";
	Shader * ourShader = new Shader(changeX, changeY);
	Field vecField(NUM_VERTICES, changeX.c_str(), changeY.c_str());
	vecField.SetEquations(changeX, changeY);
	float * vertices = NULL;
	float scalingFactor = 0.001f;
	vecField.Generate(vertices, scalingFactor);
	initialize(vao, *ourShader, vertices);
	
	double currX = 0;
	double currY = 0;
	double initialX = 0;
	double initialY = 0;

	char* dx = new char[200];
	char* dy = new char[200];
	strcpy(dx, changeX.c_str());
	strcpy(dy, changeY.c_str());
	float tempPixel = pixelSize;
	int renderedPixels = NUM_PIXELS;
	int tempPixels = NUM_PIXELS;
	float tempScale = 1.0f;

	while (!glfwWindowShouldClose(window)) {
		// Clear canvas for new render
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// If right mouse button is clicked, handle panning behavior
		int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (mouseState == GLFW_PRESS && initialX + currX  <= windowHeight && initialY + currY <= windowHeight) {
			glfwGetCursorPos(window, &currX, &currY);
			ourShader->setFloat("currX", scalingFactor * 2.00f * (float)(currX - initialX));
			ourShader->setFloat("currY", scalingFactor * 2.00f * -(float)(currY - initialY));
			currX -= initialX;
			currY -= initialY;
		}
		else {
			glfwGetCursorPos(window, &initialX, &initialY);
			initialX -= currX;
			initialY -= currY;
		}
		
		ourShader->setFloat("scalingFactor", scalingFactor);
		ourShader->setFloat("pixelSize", pixelSize);

		processInput(window);
		vecField.Generate(vertices, scalingFactor);
		display(vao, *ourShader, vertices, renderedPixels);

		// Imgui Stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		
		ImGui::NewFrame();
		ImGui::Begin("Enter Functions (ESC to Close)");
		ImGui::SetWindowSize(FwindowSize);
		ImGui::SetWindowPos(FwindowPos);
		ImGui::SetWindowFontScale(windowHeight / 1080);
		ImGui::InputText("dx", dx, 200);
		ImGui::InputText("dy", dy, 200);
		if (ImGui::Button("Save")) {
			changeX = dx;
			changeY = dy;
			vecField.SetEquations(changeX, changeY);

			delete ourShader;
			ourShader = new Shader(changeX, changeY);
		}
		ImGui::End();
		ImGui::Begin("Options");
		ImGui::SetWindowSize(OwindowSize);
		ImGui::SetWindowPos(OwindowPos);
		ImGui::SetWindowFontScale(windowHeight / 1080);
		ImGui::InputFloat("Pixel Size (Max 100)", &tempPixel, 0, 0);
		ImGui::InputInt("Number of Pixels (Max 40000)", &tempPixels, 0, 0);
		ImGui::InputFloat("Speed", &tempScale, 0, 0);
		if (ImGui::Button("Update")) {
			pixelSize = tempPixel <= 100 ? tempPixel : 100;
			renderedPixels = tempPixels <= 40000 ? tempPixels : 40000 ;
			scalingFactor = ((tempScale * tempScale <= 100.0f) && tempScale != 0 ? tempScale * tempScale : 100.0f) * 0.001f;
		}
		ImGui::End();
		ImGui::Begin("Info");
		ImGui::SetWindowSize(IwindowSize);
		ImGui::SetWindowPos(IwindowPos);
		ImGui::SetWindowFontScale(windowHeight / 1080);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	delete [] vertices;
	delete ourShader;
	delete [] dx;
	delete [] dy;
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

void initialize(unsigned int& vao, Shader& ourShader, float*& vertices) {
	glEnable(GL_PROGRAM_POINT_SIZE);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * NUM_PIXELS * sizeof(float), vertices, GL_DYNAMIC_DRAW);

	int position = glGetAttribLocation(ourShader.ID, "aPos");

	glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(position);

	int timePos = glGetAttribLocation(ourShader.ID, "time");
	glVertexAttribPointer(timePos, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(timePos);

	int fadePos = glGetAttribLocation(ourShader.ID, "fade");
	glVertexAttribPointer(fadePos, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(fadePos);
}

void display(unsigned int& vao, Shader& ourShader, float * vertices, int renderedPixels) {
	glBindVertexArray(vao);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * NUM_PIXELS * sizeof(float), vertices);
	ourShader.setFloat("Scale", (float)y_offset);
	ourShader.use();
	glDrawArrays(GL_POINTS, 0, renderedPixels);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if(y_offset - yoffset < 5 && y_offset - yoffset >=1)
		y_offset -= yoffset;
	std::cout << y_offset << std::endl;
}

