/*--------------------------------------------------------------------------------------------------------

Includes

--------------------------------------------------------------------------------------------------------*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "Shader.h"
#include "Field.h"
#include "Point.h"
#include <time.h>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"
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

void set_equations(const char * changeX, const char * changeY);

double y_offset = 1;//current x offset from scroll wheel

int main(void)
{
/*--------------------------------------------------------------------------------------------------------

GLFW Window Setup

--------------------------------------------------------------------------------------------------------*/
	srand(time(0));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int sizeOfWindow = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

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

Initialize ImGui context

--------------------------------------------------------------------------------------------------------*/
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
	ImVec2 windowSize = ImVec2(sizeOfWindow / 2, (sizeOfWindow / 20) * 2);
/*--------------------------------------------------------------------------------------------------------

OpenGL Setup

--------------------------------------------------------------------------------------------------------*/

	glViewport(0, 0, sizeOfWindow, sizeOfWindow);
	glEnable(GL_PROGRAM_POINT_SIZE);

	unsigned int vao;
	std::string changeX = "(x + y)";
	std::string changeY = "(y - x)";
	set_equations(changeX.c_str(), changeY.c_str());
	Shader  * ourShader = new Shader("src/VectorShaders/shader.vs", "src/VectorShaders/shader.fs", "src/VectorShaders/shader.gs");
	Field vecField(numVertices, changeX.c_str(), changeY.c_str());
	vecField.SetEquations(changeX, changeY);
	float* vertices = 0;
	vecField.Generate(vertices);
	initialize(vao, *ourShader, vertices);
	
	double currX = 0;
	double currY = 0;
	unsigned int xLoc = glGetUniformLocation(ourShader->ID, "currX");
	unsigned int yLoc = glGetUniformLocation(ourShader->ID, "currY");
	unsigned int scalingLoc = glGetUniformLocation(ourShader->ID, "scalingFactor");
	double initialX = 0;
	double initialY = 0;
	float boolean = 1;

	changeX = "(cos(4 * ((x*x) + (y*y))))";
	changeY = "((y*y) - (x*x))";

	char* dx = new char[200];
	char* dy = new char[200];
	strcpy(dx, "");
	strcpy(dy, "");

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
		ourShader->setFloat("pixelSize", pixelSize);

		processInput(window);
		vecField.Generate(vertices);
		display(vao, *ourShader, vertices);

		//Imgui Stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		
		ImGui::NewFrame();
		ImGui::Begin("Enter Function");
		ImGui::SetWindowSize(windowSize);
		ImGui::SetWindowFontScale(sizeOfWindow / 1080);
		ImGui::InputText("dx", dx, 200);
		ImGui::InputText("dy", dy, 200);
		if (ImGui::Button("Save"))
			boolean = 0;
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (boolean == 0) {
			changeX = dx;
			changeY = dy;
			set_equations(changeX.c_str(), changeY.c_str());
			vecField.SetEquations(changeX, changeY);

			delete ourShader;
			ourShader = new Shader("src/VectorShaders/shader.vs", "src/VectorShaders/shader.fs", "src/VectorShaders/shader.gs");
			xLoc = glGetUniformLocation(ourShader->ID, "currX");
			yLoc = glGetUniformLocation(ourShader->ID, "currY");
			scalingLoc = glGetUniformLocation(ourShader->ID, "scalingFactor");
			boolean = 1;
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
/*--------------------------------------------------------------------------------------------------------

Cleanup

--------------------------------------------------------------------------------------------------------*/
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	delete[] vertices;
	delete ourShader;
	delete[] dx;
	delete[] dy;
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

void set_equations(const char* changeX, const char* changeY) {
	//io
	std::ifstream in;
	in.open("src/VectorShaders/shader.gs");

	std::ofstream out;
	out.open("temp.txt");
	char temp[200];
	for (int i = 0; i < 50; i++) {
		in.getline(temp, 200);
		out << temp;
		out << '\n';
	}

	in.getline(temp, 200);
	out << "		x -= scalingFactor * " << changeX << ";\n";
	in.getline(temp, 200);
	out << "		y -= scalingFactor * " << changeY << ";\n";

	for (int i = 0; i < 3; i++) {
		in.getline(temp, 200);
		out << temp;
		out << '\n';
	}

	in.close();
	out.close();

	remove("src/VectorShaders/shader.gs");
	rename("temp.txt", "src/VectorShaders/shader.gs");

}