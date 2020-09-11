#version 430 core
out vec4 FragColor;
in vec4 ColorVec;

void main()
{
	FragColor = ColorVec;//vec4(1.0f, 0.0f, 1.0f, 0.5f);
}