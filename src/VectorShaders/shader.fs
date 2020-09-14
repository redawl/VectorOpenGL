#version 430 core
out vec4 FragColor;
in vec4 ColorVec;

void main()
{
	FragColor = ColorVec;
}